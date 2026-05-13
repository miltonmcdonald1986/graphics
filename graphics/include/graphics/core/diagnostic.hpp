#ifndef GRAPHICS_CORE_DIAGNOSTIC_HPP
#define GRAPHICS_CORE_DIAGNOSTIC_HPP

#include <source_location>
#include <string>

#include "diagnostic_category.hpp"

namespace graphics::core
{

	struct Diagnostic
	{
		DiagnosticCategory		category{ DiagnosticCategory::Unknown };
		std::source_location	location;
		std::string				message;
	};

	auto create_diagnostic_message(
		const Diagnostic& diagnostic
	) -> std::string;

	auto create_diagnostic_message(
		DiagnosticCategory category, 
		const std::string& message, 
		std::source_location location = std::source_location::current()
	) -> std::string;

}

#endif // GRAPHICS_CORE_DIAGNOSTIC_HPP