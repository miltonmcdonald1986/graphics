#include <graphics/platform/i_platform.hpp>

#include <algorithm>
#include <format>
#include <optional>

#include <graphics/core/expected.hpp>
#include <graphics/platform/backend.hpp>

#include "platform_glfw3.hpp"
#include "platform_sdl2.hpp"

namespace
{

using graphics::core::DiagnosticCategory;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
using graphics::platform::create_platform_glfw;
using graphics::platform::IPlatform;
using graphics::window::IWindow;
using graphics::window::WindowDesc;
using std::format;
using std::uint32_t;
using std::unique_ptr;

struct Handle
{
    uint32_t id{};
    uint32_t gen{};
};

auto create_platform_default() -> unique_ptr<IPlatform>
{
    return create_platform_glfw();
}

auto pack_handle (uint32_t id, uint32_t gen) -> uint32_t
{
    uint32_t packed = (gen << 16) | id;
    log_diagnostic (DiagnosticCategory::Platform,
        format ("packed (id,gen)=({},{}) to {}", id, gen, packed),
        LogLevel::Trace);

    return packed;
}

auto unpack_handle (uint32_t handle) -> Handle
{
    Handle unpacked{.id = handle & 0xFFFF, .gen = handle >> 16};
    log_diagnostic (DiagnosticCategory::Platform,
        format ("unpacked {} to (id,gen)=({},{})", handle, unpacked.id,
            unpacked.gen),
        LogLevel::Trace);

    return unpacked;
}

} // namespace

namespace graphics::platform
{

auto IPlatform::create_window (const WindowDesc& desc) -> Expected<uint32_t>
{
    uint32_t id;

    log_diagnostic (DiagnosticCategory::Platform, "Creating window",
        LogLevel::Debug);

    // Reuse a free slot if available
    if (!m_free_list.empty())
    {
        id = m_free_list.back();
        m_free_list.pop_back();

        log_diagnostic (DiagnosticCategory::Platform,
            format ("Re-using slot {}", id), LogLevel::Debug);
    }
    else
    {
        // Otherwise grow the slot table
        id = static_cast<uint32_t> (m_windows.size());
        m_windows.emplace_back(); // default Slot { nullptr, 0 }

        log_diagnostic (DiagnosticCategory::Platform,
            format ("No free slots available. Creating slot {}", id),
            LogLevel::Debug);
    }

    Slot& slot = m_windows[id];

    // Backend-specific creation (GLFW, SDL2, Win32, etc.)
    slot.window = create_backend_window (desc);

    // If backend creation failed, restore slot to free list
    if (!slot.window)
    {
        m_free_list.push_back (id);
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            "Failed to create backend window");
    }

    // Use the existing generation for this slot
    uint32_t gen = slot.generation;
    uint32_t handle = pack_handle (id, gen);

    log_diagnostic (DiagnosticCategory::Platform,
        format ("Created window with id {} and gen {}. Handle: {}", id, gen,
            handle),
        LogLevel::Debug);

    return handle;
}

auto IPlatform::get_all_window_ids() const -> std::vector<uint32_t>
{
    std::vector<uint32_t> result;
    result.reserve (m_windows.size());

    for (uint32_t index = 0; index < m_windows.size(); ++index)
    {
        const Slot& slot = m_windows[index];

        if (!slot.window)
            continue; // empty slot

        result.push_back (pack_handle (index, slot.generation));
    }

    return result;
}

auto IPlatform::destroy_window (uint32_t id) -> void
{
    Handle handle{unpack_handle (id)};

    // Bounds check
    if (handle.id >= m_windows.size())
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("There is no slot with id {}", id), LogLevel::Warn);

        return;
    }

    Slot& slot = m_windows[handle.id];

    // Validate slot
    if (!slot.window)
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("There is no window with id {}", id), LogLevel::Warn);

        return;
    }

    if (slot.generation != handle.gen)
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("Trying to destroy a window using a stale handle", id),
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
    log_diagnostic (DiagnosticCategory::Platform, format ("free ids: {}", m_free_list), LogLevel::Debug);
}

auto IPlatform::has_windows() const -> bool
{
    return std::any_of (m_windows.begin(), m_windows.end(),
        [] (const Slot& slot) { return (slot.window != nullptr); });
}

auto IPlatform::poll_events() const -> void { poll_backend_events(); }

auto IPlatform::swap_buffers (std::uint32_t window) const -> void
{
    if (const std::unique_ptr<IWindow>& win =
            m_windows.at (unpack_handle (window).id).window)
    {
        swap_backend_buffers (win.get());
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("There is no window with id {}", window), LogLevel::Warn);
    }
}


auto IPlatform::window_should_close (std::uint32_t id) const -> Expected<bool>
{
    uint32_t raw_id = unpack_handle (id).id;
    if (m_windows.size() <= raw_id)
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            format ("There is no window with id {}", id), LogLevel::Warn);

    const std::unique_ptr<IWindow>& window =
        m_windows.at (unpack_handle (id).id).window;
    if (!window)
    {
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            format ("There is no window with id {}", id), LogLevel::Warn);
    }

    return window_backend_should_close (window.get());
}

auto create_platform (std::optional<Backend> o_backend) -> unique_ptr<IPlatform>
{
    if (o_backend.has_value())
    {
        switch (o_backend.value())
        {
        case Backend::GLFW:
            return create_platform_glfw();
        case Backend::SDL2:
            return create_platform_sdl2();
        default:
            return create_platform_default();
        }
    }
    else
    {
        return create_platform_default();
    }
}

} // namespace graphics::platform