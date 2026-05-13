#include <graphics/core/diagnostic.hpp>

#include <format>

namespace graphics::core
{

	auto create_diagnostic_message(
		const Diagnostic& diagnostic
	) -> std::string
	{
		return std::format(
			"[{}] {} ({}:{})", 
			translate_diagnostic_category_to_string(diagnostic.category), 
			diagnostic.message, 
			diagnostic.location.file_name(), 
			diagnostic.location.line()
		);
	}

	auto create_diagnostic_message(
		DiagnosticCategory category, 
		const std::string& message, 
		std::source_location location
	) -> std::string
	{
		Diagnostic diagnostic
		{
			.category = category,
			.location = location,
			.message = message
		};

		return create_diagnostic_message(diagnostic);
	}

}