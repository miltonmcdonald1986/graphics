#include "platform_glfw3.hpp"

#include <expected>
#include <memory>
#include <source_location>

#include <GLFW/glfw3.h>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/logging.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/status.hpp>

#include "glfw_callbacks.hpp"

namespace graphics::platform
{

	auto create_platform_glfw() -> std::unique_ptr<PlatformGLFW>
	{
		return std::make_unique<PlatformGLFW>();
	}

	auto PlatformGLFW::init() -> core::Status
	{
		glfwSetErrorCallback(glfw_error_callback);

		if (glfwInit() == GLFW_TRUE)
		{
			core::log_message(
				core::LogLevel::Info,
				core::create_diagnostic_message(
					core::DiagnosticCategory::Platform,
					"Initialized GLFW"));
		}
		else
		{
			const core::Diagnostic error
			{
				.category = core::DiagnosticCategory::Platform,
				.location = std::source_location::current(),
				.message = "Failed to Initialize GLFW"
			};

			core::log_message(
				core::LogLevel::Error, 
				core::create_diagnostic_message(error));
			
			return std::unexpected(error);
		}

		m_initialized = true;
		return {};
	}

	auto PlatformGLFW::shutdown() -> core::Status
	{
		if (m_initialized)
		{
			core::log_message(
				core::LogLevel::Info,
				core::create_diagnostic_message(
					core::DiagnosticCategory::Platform,
					"Shutting down GLFW"));

			glfwTerminate();
		}
		else
		{
			core::log_message(
				core::LogLevel::Warn,
				core::create_diagnostic_message(
					core::DiagnosticCategory::Platform,
					"GLFW is not initialized; skipping shutdown"));
		}

		return {};
	}

} // namespace graphics::platform