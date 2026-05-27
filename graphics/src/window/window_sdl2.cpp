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
    uint32_t flags = SDL_WINDOW_OPENGL;
    if (desc.fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    return flags;
}

} // namespace

namespace graphics::window
{

WindowSDL2::WindowSDL2 (const WindowDesc& desc, SDL_GLContext shared_context)
    : m_window (SDL_CreateWindow (desc.title.c_str(),
          desc.position.x,
          desc.position.y,
          desc.width,
          desc.height,
          make_sdl_window_flags (desc)))
{
    if (!m_window)
        return;

    if (shared_context)
    {
        SDL_GL_SetAttribute (SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    }

    m_context = SDL_GL_CreateContext (m_window);
}

WindowSDL2::~WindowSDL2()
{
    m_context = nullptr;
    m_window = nullptr;
}

auto WindowSDL2::get_context() const -> SDL_GLContext { return m_context; }

auto WindowSDL2::get_sdl2_window() const -> SDL_Window* { return m_window; }

auto WindowSDL2::is_initialized() const -> bool
{
    return ((m_window != nullptr) && (m_context != nullptr));
}

auto WindowSDL2::set_should_close (bool should_close) -> void
{
    m_should_close = should_close;
}

auto WindowSDL2::should_close() const -> bool { return m_should_close; }

} // namespace graphics::window