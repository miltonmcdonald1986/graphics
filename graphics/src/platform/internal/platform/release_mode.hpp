#ifndef GRAPHICS_INTERNAL_PLATFORM_RELEASE_MODE_HPP
#define GRAPHICS_INTERNAL_PLATFORM_RELEASE_MODE_HPP

namespace graphics::platform
{

enum class ReleaseMode
{
    AfterDestroy,
    AfterFailedCreate
};

}

#endif // GRAPHICS_INTERNAL_PLATFORM_RELEASE_MODE_HPP