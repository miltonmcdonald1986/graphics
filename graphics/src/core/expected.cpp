#include <graphics/core/expected.hpp>

#include <graphics/core/logging.hpp>

namespace graphics::core
{

	auto create_unexpected(const Diagnostic& diagnostic) -> Unexpected
	{
		core::log_diagnostic(diagnostic);
		return std::unexpected(diagnostic);
	}

	auto create_unexpected(
		DiagnosticCategory category,
		const std::string& message,
		LogLevel level,
		std::source_location location) -> Unexpected
	{
		const Diagnostic diagnostic
		{
			.category = category,
			.level = level,
			.location = location,
			.message = message
		};

		return create_unexpected(diagnostic);
	}

} // namespace graphics::core