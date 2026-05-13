#ifndef GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP
#define GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP

#include <string>

namespace graphics::core
{

	enum class DiagnosticCategory
	{
		Core,
		Platform,
		Unknown
	};

	auto translate_diagnostic_category_to_string(
		DiagnosticCategory category
	) -> std::string;

}

#endif // GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP