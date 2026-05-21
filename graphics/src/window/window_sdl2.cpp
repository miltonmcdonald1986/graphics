#include <internal/window/window_sdl2.hpp>

#include <cstdint>

#include <SDL2/SDL_video.h>

#include <graphics/window/window_desc.hpp>

using graphics::window::WindowDesc;
using std::uint32_t;

namespace
{

auto make_sdl_window_flags (const WindowDesc& desc) -> uint32_t
{
    uint32_t flags = 0;
    if (desc.fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    return flags;
}

} // namespace

namespace graphics::window
{

WindowSDL2::WindowSDL2 (const WindowDesc& desc)
    : m_window (SDL_CreateWindow (desc.title.c_str(),
          desc.position.x,
          desc.position.y,
          desc.width,
          desc.height,
          make_sdl_window_flags (desc)))
{
}

WindowSDL2::~WindowSDL2() { m_window = nullptr; }

auto WindowSDL2::get_sdl2_window() const -> SDL_Window* { return m_window; }

auto WindowSDL2::is_initialized() const -> bool
{
    return (m_window != nullptr);
}

auto WindowSDL2::set_should_close (bool should_close) -> void
{
    m_should_close = should_close;
}

auto WindowSDL2::should_close() const -> bool { return m_should_close; }

} // namespace graphics::window