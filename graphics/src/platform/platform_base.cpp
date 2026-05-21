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

using graphics::core::DiagnosticCategory;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
using graphics::window::WindowDesc;
using std::format;
using std::uint32_t;
using std::ranges::any_of;

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
    log_diagnostic (DiagnosticCategory::Platform,
        format ("packed (index,gen)=({},{}) to {}", index, gen, packed),
        LogLevel::Trace);

    return packed;
}

auto unpack_handle (uint32_t handle) -> Handle
{
    Handle unpacked{.id = handle & INDEX_MASK, .gen = handle >> INDEX_BITS};
    log_diagnostic (DiagnosticCategory::Platform,
        format ("unpacked {} to (index,gen)=({},{})", handle, unpacked.id,
            unpacked.gen),
        LogLevel::Trace);

    return unpacked;
}

} // namespace

namespace graphics::platform
{

auto PlatformBase::create_window (const WindowDesc& desc) -> Expected<uint32_t>
{
    uint32_t win_id{};

    log_diagnostic (DiagnosticCategory::Platform, "Creating window",
        LogLevel::Debug);

    if (!m_free_list.empty())
    {
        win_id = m_free_list.back();
        m_free_list.pop_back();

        log_diagnostic (DiagnosticCategory::Platform,
            format ("Re-using slot {}", win_id), LogLevel::Debug);
    }
    else
    {
        // Otherwise grow the slot table
        win_id = static_cast<uint32_t> (m_windows.size());
        m_windows.emplace_back(); // default Slot { nullptr, 0 }

        log_diagnostic (DiagnosticCategory::Platform,
            format ("No free slots available. Creating slot {}", win_id),
            LogLevel::Debug);
    }

    Slot& slot = m_windows.at (win_id);

    // Backend-specific creation (GLFW, SDL2, Win32, etc.)
    slot.window = create_backend_window (desc);

    // If backend creation failed, restore slot to free list
    if (!slot.window)
    {
        m_free_list.push_back (win_id);
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            "Failed to create backend window");
    }

    // Use the existing generation for this slot
    uint32_t gen = slot.generation;
    uint32_t handle = pack_handle (win_id, gen);

    log_diagnostic (DiagnosticCategory::Platform,
        format ("Created window with id {} and gen {}. Handle: {}", win_id, gen,
            handle),
        LogLevel::Debug);

    return handle;
}

auto PlatformBase::destroy_window (uint32_t win_id) -> void
{
    Handle handle{unpack_handle (win_id)};

    // Bounds check
    if (handle.id >= m_windows.size())
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("There is no slot with id {}", win_id), LogLevel::Warn);

        return;
    }

    Slot& slot = m_windows.at (handle.id);

    // Validate slot
    if (!slot.window)
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("There is no window with id {}", win_id), LogLevel::Warn);

        return;
    }

    if (slot.generation != handle.gen)
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("Trying to destroy a window using a stale handle", win_id),
            LogLevel::Warn);

        return;
    }

    // Backend-specific destruction
    destroy_backend_window (slot.window.get());

    // Clear the unique_ptr
    slot.window.reset();

    // Bump generation to invalidate old handles
    slot.generation++;
    log_diagnostic (DiagnosticCategory::Platform,
        format ("Bumping generation to {} to invalidate old handles",
            slot.generation),
        LogLevel::Debug);

    // Add slot back to free list
    m_free_list.push_back (handle.id);
    log_diagnostic (DiagnosticCategory::Platform,
        format ("Slot {} is now free", handle.id), LogLevel::Debug);

    // Generate free list string for diagnostic purposes.
    if (!m_free_list.empty())
    {
        std::string ids = "free ids: [";
        ids += " ";
        ids += std::to_string (m_free_list.front());
        for (int count = 1; count < m_free_list.size(); ++count)
        {
            ids += ", ";
            ids += std::to_string (m_free_list.at (count));
        }
        ids += " ]";

        log_diagnostic (DiagnosticCategory::Platform, ids, LogLevel::Debug);
    }
}

auto PlatformBase::get_all_window_ids() const -> std::vector<uint32_t>
{
    std::vector<uint32_t> result;
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

auto PlatformBase::get_window_ptr (std::uint32_t window_id) const
    -> window::IWindow*
{
    return m_windows.at (unpack_handle (window_id).id).window.get();
}

auto PlatformBase::has_windows() const -> bool
{
    return any_of (m_windows,
        [] (const Slot& slot) -> bool { return (slot.window != nullptr); });
}

auto PlatformBase::poll_events() -> void { poll_backend_events(); }

auto PlatformBase::set_window_should_close (std::uint32_t win_id,
    bool should_close) -> void
{
    set_backend_window_should_close (get_window_ptr (win_id), should_close);
}

auto PlatformBase::swap_buffers (std::uint32_t window) const -> void
{
    swap_backend_buffers (get_window_ptr (window));
}

auto PlatformBase::window_should_close (std::uint32_t win_id) const
    -> Expected<bool>
{
    return window_backend_should_close (get_window_ptr (win_id));
}

} // namespace graphics::platform