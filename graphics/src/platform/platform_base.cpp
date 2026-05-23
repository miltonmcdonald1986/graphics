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

#include "internal/platform/handle.hpp"
#include "internal/platform/release_mode.hpp"
#include "internal/platform/slot.hpp"

using graphics::core::create_unexpected;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::DiagnosticCategory::Platform;
using graphics::core::LogLevel::Debug;
using graphics::core::LogLevel::Warn;
using graphics::platform::ReleaseMode::AfterDestroy;
using graphics::platform::ReleaseMode::AfterFailedCreate;
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

auto PlatformBase::create_window (const WindowDesc& desc) -> Expected<uint32_t>
{
    log_diagnostic (Platform, "Creating window", Debug);

    // Step 1: Acquire slot
    uint32_t win_id = acquire_slot();
    Slot& slot = m_windows.at (win_id);

    // Step 2: Backend creation
    slot.window = create_backend_window (desc);
    if (!slot.window)
    {
        release_slot (win_id, AfterFailedCreate);
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
    // Step 1: Unpack handle
    const Handle handle{unpack_handle (win_id)};
    const Slot* slot = get_slot_from_handle (handle);
    if (slot == nullptr)
    {
        log_diagnostic (Platform, format ("Invalid window handle {}", win_id),
            Warn);

        return;
    }

    // Step 2: Backend destruction
    destroy_backend_window (slot->window.get());

    // Step 3: Release slot
    release_slot (handle.id, AfterDestroy);
}

auto PlatformBase::get_all_window_ids() const -> vector<uint32_t>
{
    vector<uint32_t> result;
    result.reserve (m_windows.size());

    for (uint32_t index = 0; index < m_windows.size(); ++index)
    {
        if (m_windows.at (index).window)
        {
            result.push_back (pack_handle (index,
                m_windows.at (index).generation));
        }
    }

    return result;
}

auto PlatformBase::get_window_ptr (uint32_t window_id) const -> IWindow*
{
    if (const auto* slot = get_slot_from_handle (unpack_handle (window_id)))
    {
        return slot->window.get();
    }

    log_diagnostic (Platform,
        format ("Invalid or stale window handle {}", window_id), Warn);

    return nullptr;
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

auto PlatformBase::get_slot_from_handle (const Handle& handle) const -> const Slot*
{
    if (handle.id >= m_windows.size())
    {
        return nullptr;
    }

    const Slot& slot = m_windows.at(handle.id);
    if (!slot.window)
    {
        return nullptr;
    }

    if (slot.generation != handle.gen)
    {
        return nullptr;
    }

    return &slot;
}

auto PlatformBase::log_free_list() const -> void
{
    if (m_free_list.empty())
    {
        log_diagnostic (Platform, "free ids: [ ]", Debug);
        return;
    }

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

auto PlatformBase::release_slot (uint32_t win_id, ReleaseMode mode) -> void
{
    if (win_id >= m_windows.size())
    {
        log_diagnostic (Platform, format ("Invalid slot id {}", win_id), Warn);
        return;
    }

    Slot& slot = m_windows[win_id];

    // Clear the window pointer (safe even if nullptr)
    slot.window.reset();

    if (mode == AfterDestroy)
    {
        slot.generation++;
        log_diagnostic (Platform,
            format ("Bumping generation to {} to invalidate old handles",
                slot.generation),
            Debug);
    }

    // Return slot to free list
    m_free_list.push_back (win_id);
    log_diagnostic (Platform, format ("Slot {} is now free", win_id), Debug);

    log_free_list();
}

} // namespace graphics::platform