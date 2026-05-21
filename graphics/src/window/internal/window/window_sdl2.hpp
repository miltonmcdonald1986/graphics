#ifndef GRAPHICS_WINDOW_INTERNAL_WINDOW_SDL2_HPP
#define GRAPHICS_WINDOW_INTERNAL_WINDOW_SDL2_HPP

#include <internal/window/window_base.hpp>

#include <SDL2/SDL_video.h>

#include <graphics/window/window_desc.hpp>

namespace graphics::window
{

class WindowSDL2 final : public WindowBase
{
  public:
    explicit WindowSDL2 (const WindowDesc& desc);
    ~WindowSDL2() override;

    WindowSDL2 (const WindowSDL2&) = delete;
    auto operator= (const WindowSDL2&) -> WindowSDL2& = delete;

    WindowSDL2 (WindowSDL2&&) = delete;
    auto operator= (WindowSDL2&&) -> WindowSDL2& = delete;

    [[nodiscard]] auto get_sdl2_window() const -> SDL_Window*;
    [[nodiscard]] auto is_initialized() const -> bool;
    auto set_should_close (bool should_close) -> void;

  private:
    SDL_Window* m_window{nullptr};
};

} // namespace graphics::window

#endif // GRAPHICS_WINDOW_INTERNAL_WINDOW_SDL2_HPP