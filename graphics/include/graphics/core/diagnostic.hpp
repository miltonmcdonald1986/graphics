#ifndef GRAPHICS_CORE_DIAGNOSTIC_HPP
#define GRAPHICS_CORE_DIAGNOSTIC_HPP

#include <source_location>
#include <string>

#include "diagnostic_category.hpp"
#include "log_level.hpp"

namespace graphics::core
{

	struct Diagnostic
	{
		DiagnosticCategory category { DiagnosticCategory::Unknown };
		LogLevel level { LogLevel::Error };
		std::source_location location;
		std::string message;
	};

	auto log_diagnostic(const Diagnostic& diagnostic) -> void;

	auto log_diagnostic(
		DiagnosticCategory category, 
		const std::string& message, 
		LogLevel level = LogLevel::Error,
		std::source_location location = std::source_location::current()
	) -> void;

} // namespace graphics::core

#endif // GRAPHICS_CORE_DIAGNOSTIC_HPP