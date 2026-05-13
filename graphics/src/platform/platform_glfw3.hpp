#ifndef GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP
#define GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP

#include <graphics/platform/i_platform.hpp>

namespace graphics::platform
{

	class PlatformGLFW final : public IPlatform
	{

	public:

		~PlatformGLFW() override = default;

		auto init()		-> core::Status override;
		auto shutdown() -> core::Status override;

	private:

		bool m_initialized = false;

	};

	auto create_platform_glfw() -> std::unique_ptr<PlatformGLFW>;

}

#endif // GRAPHICS_PLATFORM_PLATFORM_GLFW3_HPP