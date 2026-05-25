#ifndef GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP
#define GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP

#include <GLFW/glfw3.h>

#include "platform_base.hpp"

namespace graphics::platform
{

class PlatformGLFW final : public PlatformBase
{

  public:
    PlatformGLFW();
    ~PlatformGLFW() override;

    PlatformGLFW (const PlatformGLFW&) = delete;
    auto operator= (const PlatformGLFW&) -> PlatformGLFW& = delete;

    PlatformGLFW (PlatformGLFW&&) = delete;
    auto operator= (PlatformGLFW&&) -> PlatformGLFW& = delete;

  protected:
    [[nodiscard]] auto backend_create_window (
        const window::WindowDesc& desc
    ) const -> std::unique_ptr<window::IWindow> override;
    auto backend_destroy_window (window::IWindow* window) const
        -> void override;
    auto backend_poll_events() -> void override;
    auto backend_set_window_should_close (window::IWindow* window,
        bool should_close) -> void override;
    auto backend_swap_buffers (window::IWindow* window) const -> void override;
    auto backend_window_should_close (window::IWindow* window) const
        -> core::Expected<bool> override;

  private:
    bool m_initialized = false;
};

auto create_platform_glfw() -> std::unique_ptr<PlatformGLFW>;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP