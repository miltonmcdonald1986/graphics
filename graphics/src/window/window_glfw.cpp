#include <graphics_internal/window/window_glfw.hpp>

#include <GLFW/glfw3.h>

#include <graphics/window/window_desc.hpp>

namespace graphics::window
{

WindowGLFW::WindowGLFW (const WindowDesc& desc)
{
    m_window = glfwCreateWindow (desc.width, desc.height, desc.title.c_str(),
        desc.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
}

WindowGLFW::~WindowGLFW() { m_window = nullptr; }

auto WindowGLFW::should_close() const -> bool { return m_should_close; }

auto WindowGLFW::get_glfw_window() const -> GLFWwindow* { return m_window; }

auto WindowGLFW::is_initialized() const -> bool
{
    return (m_window != nullptr);
}

} // namespace graphics::window