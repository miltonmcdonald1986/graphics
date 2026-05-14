#include <graphics/platform/i_platform.hpp>

#include <optional>

#include <graphics/core/expected.hpp>
#include <graphics/platform/backend.hpp>

#include "platform_glfw3.hpp"

namespace
{

using graphics::core::Expected;
using graphics::platform::create_platform_glfw;
using graphics::platform::PlatformPtr;

auto create_platform_default() -> Expected<PlatformPtr>
{
    return create_platform_glfw();
}

} // namespace

namespace graphics::platform
{

auto create_platform (std::optional<Backend> o_backend)
    -> core::Expected<PlatformPtr>
{
    if (o_backend.has_value())
    {
        switch (o_backend.value())
        {
        case Backend::GLFW:
            return create_platform_glfw();
        default:
            return create_platform_default();
        }
    }
    else
    {
        return create_platform_default();
    }
}

} // namespace graphics::platform