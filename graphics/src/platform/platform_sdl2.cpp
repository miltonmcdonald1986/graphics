#include <internal/platform/platform_sdl2.hpp>

#include <cstdio>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

#include <internal/platform/gl_includes.hpp>
#include <internal/window/window_sdl2.hpp>

using graphics::core::create_unexpected;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
using graphics::core::DiagnosticCategory::Platform;
using graphics::window::IWindow;
using graphics::window::WindowDesc;
using graphics::window::WindowSDL2;
using std::make_unique;
using std::string;
using std::unique_ptr;

namespace graphics::platform
{

auto create_platform_sdl2() -> unique_ptr<PlatformSDL2>
{
    try
    {
        return std::make_unique<PlatformSDL2>();
    }
    catch (...)
    {
        log_diagnostic (Platform, "Failed to create SDL2 platform");

        return nullptr;
    }
}

PlatformSDL2::PlatformSDL2()
{
    if (SDL_Init (SDL_INIT_VIDEO) == 0)
    {
        log_diagnostic (Platform, "Initialized SDL2", LogLevel::Info);

        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);

        m_initialized = true;
    }
    else
    {
        log_diagnostic (Platform,
            string ("Failed to initialize SDL2: ") + SDL_GetError(),
            LogLevel::Error);

        m_initialized = false;
    }
}

PlatformSDL2::~PlatformSDL2()
{
    try
    {
        if (m_initialized)
        {
            log_diagnostic (Platform, "Shutting down SDL2", LogLevel::Info);

            SDL_Quit();
        }
        else
        {
            log_diagnostic (Platform,
                "SDL2 is not initialized; skipping shutdown", LogLevel::Warn);
        }
    }
    catch (...)
    {
        puts ("An exception was caught while destroying PlatformSDL2");
    }
}

auto PlatformSDL2::backend_create_window (const WindowDesc& desc)
    -> unique_ptr<IWindow>
{
    SDL_GLContext share = m_master_context;

    auto window = make_unique<WindowSDL2> (desc, share);
    if (!window->is_initialized())
    {
        log_diagnostic (Platform, "Window is not initialized");
        return nullptr;
    }

    SDL_GLContext context = window->get_context();

    if (!m_master_context)
    {
        m_master_context = context;

        backend_make_context_current (window.get());

        if (!m_gl_loaded)
        {
            if (gl3wInit() != 0)
            {
                log_diagnostic (Platform, "Failed to initialize gl3w");
                return nullptr;
            }

            m_gl_loaded = true;
        }
    }

    return window;
}

auto PlatformSDL2::backend_destroy_window (IWindow* window) const -> void
{
    if (auto* window_sdl = dynamic_cast<WindowSDL2*> (window))
    {
        if (SDL_Window* sdl_window = window_sdl->get_sdl2_window())
        {
            SDL_DestroyWindow (sdl_window);
        }
        else
        {
            log_diagnostic (Platform,
                "Attempting to destroy a nonexistent SDL_Window",
                LogLevel::Warn);
        }

        if (SDL_GLContext context = window_sdl->get_context())
        {
            SDL_GL_DeleteContext (context);
        }
        else
        {
            log_diagnostic (Platform,
                "Attempting to delete a nonexistent SDL_GLContext",
                LogLevel::Warn);
        }
    }
    else
    {
        log_diagnostic (Platform,
            "Either the desired window does not exist or it is not an SDL2 "
            "window",
            LogLevel::Warn);
    }
}

auto PlatformSDL2::backend_make_context_current (window::IWindow* window) const
    -> core::Status
{
    if (auto* window_sdl2 = dynamic_cast<WindowSDL2*> (window))
    {
        SDL_GL_MakeCurrent (window_sdl2->get_sdl2_window(),
            window_sdl2->get_context());

        return {};
    }
    else
    {
        return create_unexpected (Platform,
            "Failed to cast window to WindowSDL2");
    }
}

auto PlatformSDL2::backend_poll_events() -> void
{
    SDL_Event event;
    while (SDL_PollEvent (&event) == 1)
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            log_diagnostic (Platform, "Received SDL_QUIT event",
                LogLevel::Info);
            // Global quit. Tell all windows to close
            for (auto win_id : get_all_window_ids())
            {
                set_window_should_close (win_id, true);
            }
            break;
        }
        case SDL_WINDOWEVENT:
        {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                // Find the specific window and mark it
                for (auto win_id : get_all_window_ids())
                {
                    if (auto* window =
                            dynamic_cast<WindowSDL2*> (get_window_ptr (win_id)))
                    {
                        if (SDL_GetWindowID (window->get_sdl2_window()) ==
                            event.window.windowID)
                        {
                            window->set_should_close (true);
                        }
                    }
                }
            }
            break;
        }
        default:
        {
            // For now, we just ignore other events. In a full implementation,
            // we would handle input events, window events, etc.
            break;
        }
        }
    }
}

auto PlatformSDL2::backend_set_window_should_close (window::IWindow* window,
    bool should_close) -> void
{
    if (auto* window_sdl2 = dynamic_cast<WindowSDL2*> (window))
    {
        window_sdl2->set_should_close (should_close);
    }
    else
    {
        log_diagnostic (Platform, "Failed to cast window to WindowSDL2",
            LogLevel::Warn);
    }
}

auto PlatformSDL2::backend_swap_buffers (window::IWindow* window) const -> void
{
    if (auto* window_sdl2 = dynamic_cast<WindowSDL2*> (window))
    {
        SDL_GL_SwapWindow (window_sdl2->get_sdl2_window());
    }
    else
    {
        log_diagnostic (Platform, "Failed to cast window to WindowSDL2",
            LogLevel::Warn);
    }
}

auto PlatformSDL2::backend_window_should_close (IWindow* window) const
    -> Expected<bool>
{
    if (auto* window_sdl2 = dynamic_cast<WindowSDL2*> (window))
    {
        return window_sdl2->should_close();
    }

    return create_unexpected (Platform, "Failed to cast window to WindowSDL2",
        LogLevel::Warn);
}

} // namespace graphics::platform