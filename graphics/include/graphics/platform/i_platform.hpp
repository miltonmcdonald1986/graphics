#ifndef GRAPHICS_PLATFORM_I_PLATFORM_HPP
#define GRAPHICS_PLATFORM_I_PLATFORM_HPP

#include <memory>
#include <optional>

#include <graphics/core/status.hpp>
#include <graphics/platform/backend.hpp>

namespace graphics::platform
{

struct IPlatform
{
  public:
    virtual ~IPlatform() = default;
};

using PlatformPtr = std::unique_ptr<IPlatform>;
using ExpectedPlatformPtr = core::Expected<PlatformPtr>;

auto create_platform (std::optional<Backend> o_backend = std::nullopt)
    -> ExpectedPlatformPtr;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_I_PLATFORM_HPP