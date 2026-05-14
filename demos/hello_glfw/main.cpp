#include <cstdio>
#include <format>
#include <utility>

#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/logging.hpp>
#include <graphics/platform/backend.hpp>
#include <graphics/platform/i_platform.hpp>

using graphics::core::Expected;
using graphics::core::LogLevel;
using graphics::core::log_message;
using graphics::platform::Backend;
using graphics::platform::create_platform;
using graphics::platform::PlatformPtr;

auto main() -> int
{
	try
	{
		Expected<PlatformPtr> expected_platform = create_platform(Backend::GLFW);
		if (!expected_platform.has_value())
		{
			log_message(LogLevel::Error, std::format("Failed to create platform with the desired backend."));
			return 1;
		}

		PlatformPtr platform = std::move(expected_platform.value());

		return 0;
	}
	catch (...)
	{
		puts("Something went wrong.\n");
		return 1;
	}
}