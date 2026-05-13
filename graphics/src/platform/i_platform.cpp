#include <graphics/platform/i_platform.hpp>

#include "platform_glfw3.hpp"

namespace graphics::platform
{

	auto create_platform_default() -> std::unique_ptr<IPlatform>
	{
		return create_platform_glfw();
	}

	auto create_platform(std::optional<Backend> o_backend) -> std::unique_ptr<IPlatform>
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