#include <internal/platform/platform_sdl2.hpp>

#include <cstdio>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

#include <internal/window/window_sdl2.hpp>

using graphics::core::create_unexpected;
using graphics::core::DiagnosticCategory;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
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
        log_diagnostic (DiagnosticCategory::Platform,
            "Failed to create SDL2 platform");

        return nullptr;
    }
}

PlatformSDL2::PlatformSDL2()
{
    if (SDL_Init (SDL_INIT_VIDEO) == 0)
    {
        log_diagnostic (DiagnosticCategory::Platform, "Initialized SDL2",
            LogLevel::Info);

        m_initialized = true;
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
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
            log_diagnostic (DiagnosticCategory::Platform, "Shutting down SDL2",
                LogLevel::Info);

            SDL_Quit();
        }
        else
        {
            log_diagnostic (DiagnosticCategory::Platform,
                "SDL2 is not initialized; skipping shutdown", LogLevel::Warn);
        }
    }
    catch (...)
    {
        puts ("An exception was caught while destroying PlatformSDL2");
    }
}

auto PlatformSDL2::create_backend_window (const WindowDesc& desc) const
    -> unique_ptr<IWindow>
{
    auto window = make_unique<WindowSDL2> (desc);
    if (!window->is_initialized())
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Window is not initialized");

        return nullptr;
    }

    return window;
}

auto PlatformSDL2::destroy_backend_window (IWindow* window) const -> void
{
    if (auto* window_sdl = dynamic_cast<WindowSDL2*> (window))
    {
        if (SDL_Window* sdl_window = window_sdl->get_sdl2_window())
        {
            SDL_DestroyWindow (sdl_window);
        }
        else
        {
            log_diagnostic (DiagnosticCategory::Platform,
                "Attempting to destroy a nonexistent SDL_Window",
                LogLevel::Warn);
        }
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Either the desired window does not exist or it is not an SDL2 "
            "window",
            LogLevel::Warn);
    }
}

auto PlatformSDL2::poll_backend_events() -> void
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            log_diagnostic (DiagnosticCategory::Platform,
                "Received SDL_QUIT event", LogLevel::Info);
            // Global quit. Tell all windows to close
            for (auto id : get_all_window_ids())
            {
                set_window_should_close (id, true);
            }
            break;
        }
        case SDL_WINDOWEVENT:
        {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                // Find the specific window and mark it
                for (auto id : get_all_window_ids())
                {
                    if (auto* window =
                            dynamic_cast<WindowSDL2*> (get_window_ptr (id)))
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

auto PlatformSDL2::set_backend_window_should_close (window::IWindow* window,
    bool should_close) -> void
{
    if (auto* window_sdl2 = dynamic_cast<WindowSDL2*> (window))
    {
        window_sdl2->set_should_close (true);
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Failed to cast window to WindowSDL2", LogLevel::Warn);
    }
}

auto PlatformSDL2::swap_backend_buffers (window::IWindow* window) const
    -> void
{
    if (auto* window_sdl2 = dynamic_cast<WindowSDL2*> (window))
    {
        SDL_GL_SwapWindow (window_sdl2->get_sdl2_window());
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Failed to cast window to WindowSDL2", LogLevel::Warn);
    }
}

auto PlatformSDL2::window_backend_should_close (IWindow* window) const
    -> Expected<bool>
{
    if (auto* window_sdl2 = dynamic_cast<WindowSDL2*> (window))
    {
        return window_sdl2->should_close();
    }

    return create_unexpected (DiagnosticCategory::Platform,
        "Failed to cast window to WindowSDL2", LogLevel::Warn);
}

} // namespace graphics::platform