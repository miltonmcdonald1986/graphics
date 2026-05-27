#ifndef GRAPHICS_WINDOW_INTERNAL_WINDOW_GLFW3_HPP
#define GRAPHICS_WINDOW_INTERNAL_WINDOW_GLFW3_HPP

#include <graphics/window/window_desc.hpp>

#include <internal/platform/gl_includes.hpp>
#include <internal/window/window_base.hpp>

namespace graphics::window
{

class WindowGLFW3 final : public window::WindowBase
{
  public:
    WindowGLFW3 (const window::WindowDesc& desc, GLFWwindow* shared_context);
    ~WindowGLFW3() override;

    WindowGLFW3 (const WindowGLFW3&) = delete;
    auto operator= (const WindowGLFW3&) -> WindowGLFW3& = delete;

    WindowGLFW3 (WindowGLFW3&&) = delete;
    auto operator= (WindowGLFW3&&) -> WindowGLFW3& = delete;

    [[nodiscard]] auto get_glfw_window() const -> GLFWwindow*;
    [[nodiscard]] auto is_initialized() const -> bool;

  private:
    GLFWwindow* m_window{nullptr};
};

} // namespace graphics::window

#endif // GRAPHICS_PLATFORM_INTERNAL_WINDOW_GLFW3_HPP