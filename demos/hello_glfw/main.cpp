#include <format>

#include <graphics/core/logging.hpp>
#include <graphics/platform/i_platform.hpp>

auto main() -> int
{
	using namespace graphics::platform;
	using namespace graphics::core;

	std::unique_ptr<IPlatform> up_platform = create_platform(Backend::GLFW);
	if (!up_platform)
		return 1;
	
	Status status = up_platform->init();
	if (!status.has_value())
	{
		log_message(LogLevel::Error, std::format("Failed to init platform : {}", create_diagnostic_message(status.error())));
		return 1;
	}

	status = up_platform->shutdown();
	if (!status.has_value())
	{
		log_message(LogLevel::Error, std::format("Failed to shutdown platform : {}", create_diagnostic_message(status.error())));
		return 1;
	}

	return 0;
}