#include "platform_glfw3.hpp"

#include <cstdio>
#include <memory>

#include <GLFW/glfw3.h>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>

#include "glfw_callbacks.hpp"

namespace graphics::platform
{

auto create_platform_glfw() -> core::Expected<std::unique_ptr<PlatformGLFW>>
{
    try
    {
        return std::make_unique<PlatformGLFW>();
    }
    catch (...)
    {
        return core::create_unexpected (core::DiagnosticCategory::Platform,
            "Failed to create GLFW platform");
    }
}

PlatformGLFW::PlatformGLFW()
{
    glfwSetErrorCallback (glfw_error_callback);

    if (glfwInit() == GLFW_TRUE)
    {
        core::log_diagnostic (core::DiagnosticCategory::Platform,
            "Initialized GLFW", core::LogLevel::Info);

        m_initialized = true;
    }
    else
    {
        core::log_diagnostic (core::DiagnosticCategory::Platform,
            "Failed to initialize GLFW", core::LogLevel::Error);

        m_initialized = false;
    }
}

PlatformGLFW::~PlatformGLFW()
{
    try
    {
        if (m_initialized)
        {
            core::log_diagnostic (core::DiagnosticCategory::Platform,
                "Shutting fown GLFW", core::LogLevel::Info);

            glfwTerminate();
        }
        else
        {
            core::log_diagnostic (core::DiagnosticCategory::Platform,
                "GLFW is not initialized; skipping shutdown",
                core::LogLevel::Warn);
        }
    }
    catch (...)
    {
        puts ("An exception was caught while destroying PlatformGLFW");
    }
}

} // namespace graphics::platform