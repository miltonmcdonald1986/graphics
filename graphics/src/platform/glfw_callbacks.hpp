#ifndef GRAPHICS_PLATFORM_GLFW_CALLBACKS_HPP
#define GRAPHICS_PLATFORM_GLFW_CALLBACKS_HPP

namespace graphics::platform
{

	auto glfw_error_callback(
		int error_code, 
		const char* description
	) -> void;

}

#endif // GRAPHICS_PLATFORM_GLFW_CALLBACKS_HPP