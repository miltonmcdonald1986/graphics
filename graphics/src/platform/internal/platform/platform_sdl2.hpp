#ifndef GRAPHICS_PLATFORM_PLATFORM_SDL2_HPP
#define GRAPHICS_PLATFORM_PLATFORM_SDL2_HPP

#include "platform_base.hpp"

#include <SDL2/SDL_video.h>

namespace graphics::platform
{

class PlatformSDL2 final : public PlatformBase
{

  public:
    PlatformSDL2();
    ~PlatformSDL2() final;

    PlatformSDL2 (const PlatformSDL2&) = delete;
    auto operator= (const PlatformSDL2&) -> PlatformSDL2& = delete;

    PlatformSDL2 (PlatformSDL2&&) = delete;
    auto operator= (PlatformSDL2&&) -> PlatformSDL2& = delete;

  protected:
    [[nodiscard]] auto backend_create_window (const window::WindowDesc& desc)
        -> std::unique_ptr<window::IWindow> final;
    auto backend_destroy_window (window::IWindow* window) const -> void final;
    auto backend_make_context_current (window::IWindow* window) const
        -> core::Status final;
    auto backend_poll_events() -> void final;
    auto backend_set_window_should_close (window::IWindow* window,
        bool should_close) -> void final;
    auto backend_swap_buffers (window::IWindow* window) const -> void final;
    auto backend_window_should_close (window::IWindow* window) const
        -> core::Expected<bool> final;

  private:
    bool m_gl_loaded{ false };
    bool m_initialized{ false };
    SDL_GLContext m_master_context{ nullptr };
};

auto create_platform_sdl2() -> std::unique_ptr<PlatformSDL2>;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_PLATFORM_SDL2_HPP