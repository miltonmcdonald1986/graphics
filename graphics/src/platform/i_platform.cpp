#include <graphics/platform/i_platform.hpp>

#include <memory>
#include <optional>

#include <graphics/platform/backend.hpp>

#include <internal/platform/platform_glfw3.hpp>
#include <internal/platform/platform_sdl2.hpp>

using graphics::platform::create_platform_glfw;
using graphics::platform::IPlatform;
using std::unique_ptr;

namespace
{

auto create_platform_default() -> unique_ptr<IPlatform>
{
    return create_platform_glfw();
}

} // namespace

namespace graphics::platform
{

IPlatform::~IPlatform() = default;

auto create_platform (std::optional<Backend> o_backend) -> unique_ptr<IPlatform>
{
    if (o_backend.has_value())
    {
        switch (o_backend.value())
        {
        case Backend::GLFW:
            return create_platform_glfw();
        case Backend::SDL2:
            return create_platform_sdl2();
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