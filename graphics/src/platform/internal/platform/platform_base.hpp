#ifndef GRAPHICS_INTERNAL_PLATFORM_PLATFORM_BASE_HPP
#define GRAPHICS_INTERNAL_PLATFORM_PLATFORM_BASE_HPP

#include <graphics/platform/i_platform.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

#include <internal/platform/slot.hpp>

namespace graphics::platform
{

struct PlatformBase : public IPlatform
{
  public:
    ~PlatformBase() override = default;

    auto create_window (const window::WindowDesc& desc)
        -> core::Expected<std::uint32_t> final;

    auto destroy_window (std::uint32_t win_id) -> void final;

    [[nodiscard]] auto get_all_window_ids() const
        -> std::vector<std::uint32_t> final;

    [[nodiscard]] auto get_window_ptr (std::uint32_t window_id) const
        -> window::IWindow*;

    [[nodiscard]] auto has_windows() const -> bool final;

    auto poll_events() -> void final;

    auto set_window_should_close (std::uint32_t win_id, bool should_close)
        -> void final;

    auto swap_buffers (std::uint32_t window) const -> void final;

    [[nodiscard]] auto window_should_close (std::uint32_t win_id) const
        -> core::Expected<bool> final;

  protected:
    [[nodiscard]] virtual auto create_backend_window (
        const window::WindowDesc& desc
    ) const -> std::unique_ptr<window::IWindow> = 0;

    virtual auto destroy_backend_window (window::IWindow* window) const
        -> void = 0;

    virtual auto poll_backend_events() -> void = 0;

    virtual auto set_backend_window_should_close (window::IWindow* window,
        bool should_close) -> void = 0;

    virtual auto swap_backend_buffers (window::IWindow* window) const
        -> void = 0;

    virtual auto window_backend_should_close (window::IWindow* window) const
        -> core::Expected<bool> = 0;

  private:
    auto acquire_slot() -> std::uint32_t;
    auto create_backend_window_into (Slot& slot, const window::WindowDesc& desc)
        -> bool;
    auto release_slot (std::uint32_t win_id) -> void;

    std::vector<std::uint32_t> m_free_list;
    std::vector<Slot> m_windows;
};

} // namespace graphics::platform

#endif // GRAPHICS_INTERNAL_PLATFORM_PLATFORM_BASE_HPP