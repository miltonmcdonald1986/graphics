#include <internal/platform/platform_glfw3.hpp>

#include <cstdio>
#include <memory>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/status.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

#include <internal/platform/gl_includes.hpp>
#include <internal/platform/glfw_callbacks.hpp>
#include <internal/window/window_glfw3.hpp>

using graphics::core::create_unexpected;
using graphics::core::Expected;
using graphics::core::log_diagnostic;
using graphics::core::LogLevel;
using graphics::core::DiagnosticCategory::Platform;
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
        log_diagnostic (Platform, "Failed to create GLFW platform");

        return nullptr;
    }
}

PlatformGLFW::PlatformGLFW()
{
    glfwSetErrorCallback (glfw_error_callback);

    if (glfwInit() == GLFW_TRUE)
    {
        log_diagnostic (Platform, "Initialized GLFW", LogLevel::Info);

        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_initialized = true;
    }
    else
    {
        log_diagnostic (Platform, "Failed to initialize GLFW", LogLevel::Error);

        m_initialized = false;
    }
}

PlatformGLFW::~PlatformGLFW()
{
    try
    {
        if (m_initialized)
        {
            log_diagnostic (Platform, "Shutting fown GLFW", LogLevel::Info);

            glfwTerminate();
        }
        else
        {
            log_diagnostic (Platform,
                "GLFW is not initialized; skipping shutdown", LogLevel::Warn);
        }
    }
    catch (...)
    {
        puts ("An exception was caught while destroying PlatformGLFW");
    }
}

auto PlatformGLFW::backend_create_window (const window::WindowDesc& desc)
    -> unique_ptr<IWindow>
{
    GLFWwindow* shared_context = m_master_context;
    auto window = make_unique<WindowGLFW3> (desc, shared_context);
    if (!window->is_initialized())
    {
        log_diagnostic (Platform, "Window is not initialized");

        return nullptr;
    }

    GLFWwindow* glfw_window = window->get_glfw_window();

    if (m_master_context == nullptr)
    {
        m_master_context = glfw_window;

        if (!backend_make_context_current (window.get()))
        {
            log_diagnostic (Platform,
                "Failed to make master context current for window");

            return nullptr;
        }

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

auto PlatformGLFW::backend_destroy_window (window::IWindow* window) const
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
            log_diagnostic (Platform,
                "Attempting to destroy a nonexistant GLFWwindow",
                LogLevel::Warn);
        }
    }
    else
    {
        log_diagnostic (Platform,
            "Either the desired window does not exist or it is not "
            "a GLFW window",
            LogLevel::Warn);
    }
}

auto PlatformGLFW::backend_make_context_current (window::IWindow* window) const
    -> core::Status
{
    if (auto* window_glfw = dynamic_cast<WindowGLFW3*> (window))
    {
        glfwMakeContextCurrent (window_glfw->get_glfw_window());
        return {};
    }

    return create_unexpected (Platform, "Failed to cast window to WindowGLFW");
}

auto PlatformGLFW::backend_poll_events() -> void { glfwPollEvents(); }

auto PlatformGLFW::backend_set_window_should_close (window::IWindow* window,
    bool should_close) -> void
{
    if (auto* window_glfw = dynamic_cast<WindowGLFW3*> (window))
    {
        glfwSetWindowShouldClose (window_glfw->get_glfw_window(),
            should_close ? GLFW_TRUE : GLFW_FALSE);
    }
    else
    {
        log_diagnostic (Platform, "Failed to cast window to WindowGLFW",
            LogLevel::Warn);
    }
}

auto PlatformGLFW::backend_swap_buffers (window::IWindow* window) const -> void
{
    if (auto* window_glfw = dynamic_cast<WindowGLFW3*> (window))
    {
        glfwSwapBuffers (window_glfw->get_glfw_window());
    }
    else
    {
        log_diagnostic (Platform, "Failed to cast window to WindowGLFW",
            LogLevel::Warn);
    }
}

auto PlatformGLFW::backend_window_should_close (window::IWindow* window) const
    -> Expected<bool>
{
    if (auto* window_glfw = dynamic_cast<WindowGLFW3*> (window))
    {
        return glfwWindowShouldClose (window_glfw->get_glfw_window());
    }

    return create_unexpected (Platform, "Failed to cast window to WindowGLFW",
        LogLevel::Warn);
}

} // namespace graphics::platform