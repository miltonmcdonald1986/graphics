#include <internal/window/window_glfw3.hpp>

#include <GLFW/glfw3.h>

#include <graphics/window/window_desc.hpp>

namespace graphics::window
{

WindowGLFW3::WindowGLFW3 (const WindowDesc& desc)
    : m_window (glfwCreateWindow (desc.width,
          desc.height,
          desc.title.c_str(),
          desc.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
          nullptr))
{
}

WindowGLFW3::~WindowGLFW3() { m_window = nullptr; }

auto WindowGLFW3::get_glfw_window() const -> GLFWwindow* { return m_window; }

auto WindowGLFW3::is_initialized() const -> bool
{
    return (m_window != nullptr);
}

} // namespace graphics::window