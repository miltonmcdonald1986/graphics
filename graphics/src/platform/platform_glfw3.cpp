#include <internal/platform/platform_glfw3.hpp>

#include <cstdio>
#include <memory>

#include <GLFW/glfw3.h>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

#include <internal/platform/glfw_callbacks.hpp>
#include <internal/window/window_glfw3.hpp>

using graphics::core::create_unexpected;
using graphics::core::DiagnosticCategory;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
using graphics::window::IWindow;
using graphics::window::WindowGLFW3;
using std::make_unique;
using std::unique_ptr;

namespace graphics::platform
{

auto create_platform_glfw() -> unique_ptr<PlatformGLFW>
{
    try
    {
        return make_unique<PlatformGLFW>();
    }
    catch (...)
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Failed to create GLFW platform");

        return nullptr;
    }
}

PlatformGLFW::PlatformGLFW()
{
    glfwSetErrorCallback (glfw_error_callback);

    if (glfwInit() == GLFW_TRUE)
    {
        log_diagnostic (DiagnosticCategory::Platform, "Initialized GLFW",
            LogLevel::Info);

        m_initialized = true;
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Failed to initialize GLFW", LogLevel::Error);

        m_initialized = false;
    }
}

PlatformGLFW::~PlatformGLFW()
{
    try
    {
        if (m_initialized)
        {
            log_diagnostic (DiagnosticCategory::Platform, "Shutting fown GLFW",
                LogLevel::Info);

            glfwTerminate();
        }
        else
        {
            log_diagnostic (DiagnosticCategory::Platform,
                "GLFW is not initialized; skipping shutdown", LogLevel::Warn);
        }
    }
    catch (...)
    {
        puts ("An exception was caught while destroying PlatformGLFW");
    }
}

auto PlatformGLFW::create_backend_window (const window::WindowDesc& desc) const
    -> unique_ptr<IWindow>
{
    auto window = make_unique<WindowGLFW3> (desc);
    if (!window->is_initialized())
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Window is not initialized");

        return nullptr;
    }

    return window;
}

auto PlatformGLFW::destroy_backend_window (window::IWindow* window) const
    -> void
{
    if (auto* window_glfw = dynamic_cast<WindowGLFW3*> (window))
    {
        if (GLFWwindow* glfw_window = window_glfw->get_glfw_window())
        {
            glfwDestroyWindow (glfw_window);
        }
        else
        {
            log_diagnostic (DiagnosticCategory::Platform,
                "Attempting to destroy a nonexistant GLFWwindow",
                LogLevel::Warn);
        }
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Either the desired window does not exist or it is not "
            "a GLFW window",
            LogLevel::Warn);
    }
}

auto PlatformGLFW::poll_backend_events() const -> void { glfwPollEvents(); }

auto PlatformGLFW::swap_backend_buffers (window::IWindow* window) const -> void
{
    if (auto* window_glfw = dynamic_cast<WindowGLFW3*> (window))
    {
        glfwSwapBuffers (window_glfw->get_glfw_window());
    }
    else
    {
        log_diagnostic (DiagnosticCategory::Platform,
            "Failed to cast window to WindowGLFW", LogLevel::Warn);
    }
}

auto PlatformGLFW::window_backend_should_close (window::IWindow* window) const
    -> Expected<bool>
{
    if (auto* window_glfw = dynamic_cast<WindowGLFW3*> (window))
    {
        return glfwWindowShouldClose (window_glfw->get_glfw_window());
    }

    return create_unexpected (DiagnosticCategory::Platform,
        "Failed to cast window to WindowGLFW", LogLevel::Warn);
}

} // namespace graphics::platform