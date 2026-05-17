#include "platform_sdl2.hpp"

#include <SDL2/SDL.h>

namespace graphics::platform
{

auto create_platform_sdl2() -> core::Expected<std::unique_ptr<PlatformSDL2>>
{
    try
    {
        return std::make_unique<PlatformSDL2>();
    }
    catch (...)
    {
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            "Failed to create SDL2 platform");
    }
}

PlatformSDL2::PlatformSDL2() 
{
    if (SDL_Init (SDL_INIT_VIDEO) == 0)
    {
        core::log_diagnostic (core::DiagnosticCategory::Platform,
            "Initialized SDL2",
            core::LogLevel::Info);

        m_initialized = true;
    }
    else
    {
        core::log_diagnostic (core::DiagnosticCategory::Platform,
            std::string ("Failed to initialize SDL2: ") + SDL_GetError(),
            core::LogLevel::Error);

        m_initialized = false;
    }
}

PlatformSDL2::~PlatformSDL2()
{
    try
    {
        if (m_initialized)
        {
            core::log_diagnostic (core::DiagnosticCategory::Platform,
                "Shutting down SDL2", core::LogLevel::Info);

            SDL_Quit();
        }
        else
        {
            core::log_diagnostic (core::DiagnosticCategory::Platform,
                "SDL2 is not initialized; skipping shutdown",
                core::LogLevel::Warn);
        }
    }
    catch (...)
    {
        puts ("An exception was caught while destroying PlatformSDL2");
    }
}

} // namespace graphics::platform