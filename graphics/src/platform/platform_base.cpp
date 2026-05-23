#include "internal/platform/platform_base.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <vector>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

#include "internal/platform/slot.hpp"

using graphics::core::create_unexpected;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::DiagnosticCategory::Platform;
using graphics::core::LogLevel::Debug;
using graphics::core::LogLevel::Trace;
using graphics::core::LogLevel::Warn;
using graphics::window::IWindow;
using graphics::window::WindowDesc;
using std::format;
using std::string;
using std::to_string;
using std::uint32_t;
using std::vector;
using std::ranges::any_of;

namespace graphics::platform
{

namespace
{

struct Handle
{
    uint32_t id{};
    uint32_t gen{};
};

constexpr uint32_t INDEX_BITS = 16;
constexpr uint32_t INDEX_MASK = (1U << INDEX_BITS) - 1U;

auto pack_handle (uint32_t index, uint32_t gen) -> uint32_t
{
    uint32_t packed = (gen << INDEX_BITS) | index;
    log_diagnostic (Platform,
        format ("packed (index,gen)=({},{}) to {}", index, gen, packed), Trace);

    return packed;
}

auto unpack_handle (uint32_t handle) -> Handle
{
    Handle unpacked{.id = handle & INDEX_MASK, .gen = handle >> INDEX_BITS};
    log_diagnostic (Platform,
        format ("unpacked {} to (index,gen)=({},{})", handle, unpacked.id,
            unpacked.gen),
        Trace);

    return unpacked;
}

} // namespace

auto PlatformBase::create_window (const WindowDesc& desc) -> Expected<uint32_t>
{
    log_diagnostic (Platform, "Creating window", Debug);

    // Step 1: Acquire slot
    uint32_t win_id = acquire_slot();
    Slot& slot = m_windows.at (win_id);

    // Step 2: Backend creation
    if (!create_backend_window_into (slot, desc))
    {
        release_slot (win_id);
        return create_unexpected (Platform, "Failed to create backend window");
    }

    // Step 3: Pack handle
    uint32_t handle = pack_handle (win_id, slot.generation);

    log_diagnostic (Platform,
        format ("Created window with id {} and gen {}. Handle: {}", win_id,
            slot.generation, handle),
        Debug);

    return handle;
}

auto PlatformBase::destroy_window (uint32_t win_id) -> void
{
    Handle handle{unpack_handle (win_id)};

    // Bounds check
    if (handle.id >= m_windows.size())
    {
        log_diagnostic (Platform,
            format ("There is no slot with id {}", win_id), Warn);

        return;
    }

    Slot& slot = m_windows.at (handle.id);

    // Validate slot
    if (!slot.window)
    {
        log_diagnostic (Platform,
            format ("There is no window with id {}", win_id), Warn);

        return;
    }

    if (slot.generation != handle.gen)
    {
        log_diagnostic (Platform,
            format ("Trying to destroy a window with stale handle {}", win_id),
            Warn);

        return;
    }

    // Backend-specific destruction
    destroy_backend_window (slot.window.get());

    // Clear the unique_ptr
    slot.window.reset();

    // Bump generation to invalidate old handles
    slot.generation++;
    log_diagnostic (Platform,
        format ("Bumping generation to {} to invalidate old handles",
            slot.generation),
        Debug);

    // Add slot back to free list
    m_free_list.push_back (handle.id);
    log_diagnostic (Platform, format ("Slot {} is now free", handle.id), Debug);

    // Generate free list string for diagnostic purposes.
    if (!m_free_list.empty())
    {
        string ids = "free ids: [";
        ids += " ";
        ids += to_string (m_free_list.front());
        for (int count = 1; count < m_free_list.size(); ++count)
        {
            ids += ", ";
            ids += to_string (m_free_list.at (count));
        }
        ids += " ]";

        log_diagnostic (Platform, ids, Debug);
    }
}

auto PlatformBase::get_all_window_ids() const -> vector<uint32_t>
{
    vector<uint32_t> result;
    result.reserve (m_windows.size());

    for (uint32_t index = 0; index < m_windows.size(); ++index)
    {
        const Slot& slot = m_windows.at (index);

        if (!slot.window)
        {
            continue; // empty slot
        }

        result.push_back (pack_handle (index, slot.generation));
    }

    return result;
}

auto PlatformBase::get_window_ptr (uint32_t window_id) const -> IWindow*
{
    const auto handle = unpack_handle (window_id);
    if (handle.id >= m_windows.size())
    {
        log_diagnostic (Platform,
            format ("Unpacked handle id {} is out of bounds", handle.id));

        return nullptr;
    }

    const Slot& slot = m_windows.at (handle.id);
    if (slot.generation != handle.gen)
    {
        log_diagnostic (Platform,
            format ("Stale handle: expected gen {}, got gen {}",
                slot.generation, handle.gen));

        return nullptr;
    }

    if (!slot.window)
    {
        log_diagnostic (Platform,
            format ("No window exists at slot id {}", handle.id), Warn);

        return nullptr;
    }

    return slot.window.get();
}

auto PlatformBase::has_windows() const -> bool
{
    return any_of (m_windows,
        [] (const Slot& slot) -> bool { return (slot.window != nullptr); });
}

auto PlatformBase::poll_events() -> void { poll_backend_events(); }

auto PlatformBase::set_window_should_close (uint32_t win_id, bool should_close)
    -> void
{
    if (auto* ptr = get_window_ptr (win_id))
    {
        set_backend_window_should_close (ptr, should_close);
        return;
    }

    log_diagnostic (Platform, "Invalid window handle; cannot set close flag",
        Warn);
}

auto PlatformBase::swap_buffers (uint32_t win_id) const -> void
{
    if (auto* ptr = get_window_ptr (win_id))
    {
        swap_backend_buffers (ptr);
        return;
    }

    log_diagnostic (Platform, "Invalid window handle; cannot swap buffers",
        Warn);
}

auto PlatformBase::window_should_close (uint32_t win_id) const -> Expected<bool>
{
    if (auto* ptr = get_window_ptr (win_id))
    {
        return window_backend_should_close (ptr);
    }

    return create_unexpected (Platform,
        "Invalid window handle; cannot check if window should close", Warn);
}

auto PlatformBase::acquire_slot() -> std::uint32_t
{
    uint32_t win_id{};

    // First try to reuse a slot from the free list
    if (!m_free_list.empty())
    {
        win_id = m_free_list.back();
        m_free_list.pop_back();

        log_diagnostic (Platform, format ("Re-using slot {}", win_id), Debug);
        return win_id;
    }

    // Otherwise grow the slot table
    win_id = static_cast<uint32_t> (m_windows.size());
    m_windows.emplace_back();

    log_diagnostic (Platform,
        format ("No free slots available. Creating slot {}", win_id), Debug);

    return win_id;
}

auto PlatformBase::create_backend_window_into (Slot& slot,
    const WindowDesc& desc) -> bool
{
    slot.window = create_backend_window (desc);
    return static_cast<bool> (slot.window);
}

auto PlatformBase::release_slot (std::uint32_t win_id) -> void
{
    m_free_list.push_back (win_id);
}

} // namespace graphics::platform