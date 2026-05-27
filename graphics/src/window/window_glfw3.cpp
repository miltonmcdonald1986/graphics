#include <internal/window/window_glfw3.hpp>

#include <graphics/window/window_desc.hpp>

#include <internal/platform/gl_includes.hpp>

namespace graphics::window
{

WindowGLFW3::WindowGLFW3 (const WindowDesc& desc, GLFWwindow* shared_context)
    : m_window (glfwCreateWindow (desc.width,
          desc.height,
          desc.title.c_str(),
          desc.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
          shared_context))
{
}

WindowGLFW3::~WindowGLFW3() { m_window = nullptr; }

auto WindowGLFW3::get_glfw_window() const -> GLFWwindow* { return m_window; }

auto WindowGLFW3::is_initialized() const -> bool
{
    return (m_window != nullptr);
}

} // namespace graphics::window