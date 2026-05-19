#ifndef GRAPHICS_INTERNAL_WINDOW_WINDOW_GLFW_HPP
#define GRAPHICS_INTERNAL_WINDOW_WINDOW_GLFW_HPP

#include <graphics/window/i_window.hpp>

#include <GLFW/glfw3.h>

#include <graphics/window/window_desc.hpp>

namespace graphics::window
{

class WindowGLFW final : public IWindow
{
  public:
    explicit WindowGLFW (const WindowDesc& desc);
    ~WindowGLFW() override;

    WindowGLFW (const WindowGLFW&) = delete;
    auto operator= (const WindowGLFW&) -> WindowGLFW& = delete;

    WindowGLFW (WindowGLFW&&) = delete;
    auto operator= (WindowGLFW&&) -> WindowGLFW& = delete;

    [[nodiscard]] auto should_close() const -> bool override;

    [[nodiscard]] auto get_glfw_window() const -> GLFWwindow*;
    [[nodiscard]] auto is_initialized() const -> bool;

  private:
    bool m_should_close{false};
    GLFWwindow* m_window{nullptr};
};

} // namespace graphics::window

#endif // GRAPHICS_INTERNAL_WINDOW_WINDOW_GLFW_HPP