#ifndef GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP
#define GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP

#include <graphics/platform/i_platform.hpp>

namespace graphics::platform
{

class PlatformGLFW final : public IPlatform
{

  public:
    PlatformGLFW();
    ~PlatformGLFW() override;

    PlatformGLFW (const PlatformGLFW&) = delete;
    auto operator= (const PlatformGLFW&) -> PlatformGLFW& = delete;

    PlatformGLFW (PlatformGLFW&&) = delete;
    auto operator= (PlatformGLFW&&) -> PlatformGLFW& = delete;

  private:
    bool m_initialized = false;
};

auto create_platform_glfw() -> core::Expected<std::unique_ptr<PlatformGLFW>>;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP