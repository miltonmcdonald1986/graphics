#include "internal/platform_sdl2.hpp"

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

using graphics::core::create_unexpected;
using graphics::core::DiagnosticCategory;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
using graphics::window::IWindow;
using graphics::window::WindowDesc;
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

auto PlatformSDL2::create_backend_window (const WindowDesc& /*desc*/) const
    -> unique_ptr<IWindow>
{
    log_diagnostic (DiagnosticCategory::Platform,
        "PlatformSDL2::create_backend_window not implemented");

    return nullptr;
}

auto PlatformSDL2::destroy_backend_window (IWindow* /*window*/) const -> void
{
    log_diagnostic (DiagnosticCategory::Platform,
        "PlatformSDL2::destroy_backend_window not implemented");
}

auto PlatformSDL2::poll_backend_events() const -> void
{
    log_diagnostic (DiagnosticCategory::Platform,
        "PlatformSDL2::poll_backend_events not implemented");
}

auto PlatformSDL2::swap_backend_buffers (window::IWindow* /*window*/) const
    -> void
{
    log_diagnostic (DiagnosticCategory::Platform,
        "PlatformSDL2::swap_backend_buffers not implemented");
}

auto PlatformSDL2::window_backend_should_close (IWindow* /*window*/) const
    -> Expected<bool>
{
    return create_unexpected (DiagnosticCategory::Platform,
        "Platform::SDL2::window_backend_should_close not implemented");
}

} // namespace graphics::platform