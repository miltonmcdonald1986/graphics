#ifndef GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP
#define GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP

#include "platform_base.hpp"

#include "gl_includes.hpp"

namespace graphics::platform
{

class PlatformGLFW final : public PlatformBase
{

  public:
    PlatformGLFW();
    ~PlatformGLFW() final;

    PlatformGLFW (const PlatformGLFW&) = delete;
    auto operator= (const PlatformGLFW&) -> PlatformGLFW& = delete;

    PlatformGLFW (PlatformGLFW&&) = delete;
    auto operator= (PlatformGLFW&&) -> PlatformGLFW& = delete;

  protected:
    [[nodiscard]] auto backend_create_window (const window::WindowDesc& desc)
        -> std::unique_ptr<window::IWindow> final;
    auto backend_destroy_window (window::IWindow* window) const -> void final;
    auto backend_make_context_current (window::IWindow* window) const
        -> core::Status final;
    auto backend_poll_events() -> void final;
    auto backend_set_window_should_close (window::IWindow* window,
        bool should_close) -> void final;
    auto backend_swap_buffers (window::IWindow* window) const -> void final;
    auto backend_window_should_close (window::IWindow* window) const
        -> core::Expected<bool> final;

  private:
    bool m_gl_loaded{ false };
    bool m_initialized{ false };
    GLFWwindow* m_master_context{ nullptr };
};

auto create_platform_glfw() -> std::unique_ptr<PlatformGLFW>;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP