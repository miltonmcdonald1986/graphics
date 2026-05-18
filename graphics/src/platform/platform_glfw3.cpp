#include "platform_glfw3.hpp"

#include <cstdio>
#include <memory>

#include <GLFW/glfw3.h>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/window/i_window.hpp>

#include "glfw_callbacks.hpp"
#include <graphics_internal/window/window_glfw.hpp>

using graphics::core::create_unexpected;
using graphics::core::DiagnosticCategory;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
using graphics::window::IWindow;
using graphics::window::WindowGLFW;
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
    auto window = make_unique<WindowGLFW> (desc);
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
    if (WindowGLFW* window_glfw = static_cast<WindowGLFW*> (window))
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
    if (WindowGLFW* window_glfw = static_cast<WindowGLFW*> (window))
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
    if (WindowGLFW* window_glfw = static_cast<WindowGLFW*> (window))
    {
        return glfwWindowShouldClose (window_glfw->get_glfw_window());
    }

    return create_unexpected (DiagnosticCategory::Platform,
        "Failed to cast window to WindowGLFW", LogLevel::Warn);
}

} // namespace graphics::platform