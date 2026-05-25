#ifndef GRAPHICS_PLATFORM_PLATFORM_SDL2_HPP
#define GRAPHICS_PLATFORM_PLATFORM_SDL2_HPP

#include "platform_base.hpp"

namespace graphics::platform
{

class PlatformSDL2 final : public PlatformBase
{

  public:
    PlatformSDL2();
    ~PlatformSDL2() override;

    PlatformSDL2 (const PlatformSDL2&) = delete;
    auto operator= (const PlatformSDL2&) -> PlatformSDL2& = delete;

    PlatformSDL2 (PlatformSDL2&&) = delete;
    auto operator= (PlatformSDL2&&) -> PlatformSDL2& = delete;

  protected:
    [[nodiscard]] auto backend_create_window (
        const window::WindowDesc& desc
    ) const -> std::unique_ptr<window::IWindow> override;
    auto backend_destroy_window (window::IWindow* window) const
        -> void override;
    auto backend_poll_events() -> void override;
    auto backend_set_window_should_close (window::IWindow* window,
        bool should_close) -> void override;
    auto backend_swap_buffers (window::IWindow* window) const -> void override;
    auto backend_window_should_close (window::IWindow* window) const
        -> core::Expected<bool> override;

  private:
    bool m_initialized{false};
};

auto create_platform_sdl2() -> std::unique_ptr<PlatformSDL2>;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_PLATFORM_SDL2_HPP