#ifndef GRAPHICS_INTERNAL_PLATFORM_RELEASE_MODE_HPP
#define GRAPHICS_INTERNAL_PLATFORM_RELEASE_MODE_HPP

#include <cstdint>

namespace graphics::platform
{

enum class ReleaseMode : std::uint8_t
{
    AfterDestroy,
    AfterFailedCreate
};

} // namespace graphics::platform

#endif // GRAPHICS_INTERNAL_PLATFORM_RELEASE_MODE_HPP