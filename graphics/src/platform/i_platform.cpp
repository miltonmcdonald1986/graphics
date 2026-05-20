#include <graphics/platform/i_platform.hpp>

#include <algorithm>
#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/platform/backend.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

#include <internal/platform/platform_glfw3.hpp>
#include <internal/platform/platform_sdl2.hpp>
#include <internal/platform/slot.hpp>

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

constexpr uint32_t INDEX_BITS = 16;
constexpr uint32_t INDEX_MASK = (1U << INDEX_BITS) - 1U;

auto create_platform_default() -> unique_ptr<IPlatform>
{
    return create_platform_glfw();
}

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

struct IPlatform::Impl
{
    std::vector<std::uint32_t> m_free_list;
    std::vector<Slot> m_windows;
};

IPlatform::IPlatform() : impl (std::make_unique<Impl>()) {}

IPlatform::~IPlatform() = default;

auto IPlatform::create_window (const WindowDesc& desc) -> Expected<uint32_t>
{
    uint32_t win_id{};

    log_diagnostic (DiagnosticCategory::Platform, "Creating window",
        LogLevel::Debug);

    auto& free_list = impl->m_free_list;
    auto& windows = impl->m_windows;

    if (!free_list.empty())
    {
        win_id = free_list.back();
        free_list.pop_back();

        log_diagnostic (DiagnosticCategory::Platform,
            format ("Re-using slot {}", win_id), LogLevel::Debug);
    }
    else
    {
        // Otherwise grow the slot table
        win_id = static_cast<uint32_t> (windows.size());
        windows.emplace_back(); // default Slot { nullptr, 0 }

        log_diagnostic (DiagnosticCategory::Platform,
            format ("No free slots available. Creating slot {}", win_id),
            LogLevel::Debug);
    }

    Slot& slot = windows.at (win_id);

    // Backend-specific creation (GLFW, SDL2, Win32, etc.)
    slot.window = create_backend_window (desc);

    // If backend creation failed, restore slot to free list
    if (!slot.window)
    {
        free_list.push_back (win_id);
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

auto IPlatform::get_all_window_ids() const -> std::vector<uint32_t>
{
    auto& windows = impl->m_windows;

    std::vector<uint32_t> result;
    result.reserve (windows.size());

    for (uint32_t index = 0; index < windows.size(); ++index)
    {
        const Slot& slot = windows.at (index);

        if (!slot.window)
        {
            continue; // empty slot
        }

        result.push_back (pack_handle (index, slot.generation));
    }

    return result;
}

auto IPlatform::destroy_window (uint32_t win_id) -> void
{
    Handle handle{unpack_handle (win_id)};
    auto& free_list = impl->m_free_list;
    auto& windows = impl->m_windows;

    // Bounds check
    if (handle.id >= windows.size())
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("There is no slot with id {}", win_id), LogLevel::Warn);

        return;
    }

    Slot& slot = windows.at (handle.id);

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
    free_list.push_back (handle.id);
    log_diagnostic (DiagnosticCategory::Platform,
        format ("Slot {} is now free", handle.id), LogLevel::Debug);

    // Generate free list string for diagnostic purposes.
    if (!free_list.empty())
    {
        std::string ids = "free ids: [";
        ids += " ";
        ids += std::to_string (free_list.front());
        for (int count = 1; count < free_list.size(); ++count)
        {
            ids += ", ";
            ids += std::to_string (free_list.at (count));
        }
        ids += " ]";

        log_diagnostic (DiagnosticCategory::Platform, ids, LogLevel::Debug);
    }
}

auto IPlatform::has_windows() const -> bool
{
    auto& windows = impl->m_windows;
    return std::ranges::any_of (windows,
        [] (const Slot& slot) -> bool { return (slot.window != nullptr); });
}

auto IPlatform::poll_events() const -> void { poll_backend_events(); }

auto IPlatform::swap_buffers (std::uint32_t window) const -> void
{
    if (const std::unique_ptr<IWindow>& win =
            impl->m_windows.at (unpack_handle (window).id).window)
    {
        swap_backend_buffers (win.get());
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            format ("There is no window with id {}", window), LogLevel::Warn);
    }
}

auto IPlatform::window_should_close (std::uint32_t win_id) const
    -> Expected<bool>
{
    auto& windows = impl->m_windows;
    const uint32_t raw_id = unpack_handle (win_id).id;
    if (windows.size() <= raw_id)
    {
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            format ("There is no window with id {}", win_id), LogLevel::Warn);
    }

    const std::unique_ptr<IWindow>& window =
        windows.at (unpack_handle (win_id).id).window;
    if (!window)
    {
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            format ("There is no window with id {}", win_id), LogLevel::Warn);
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