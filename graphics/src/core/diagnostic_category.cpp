#include <graphics_internal/core/diagnostic_category.hpp>

#include <string>

namespace graphics::core
{

auto translate_diagnostic_category_to_string (DiagnosticCategory category)
    -> std::string
{
    switch (category)
    {
    case DiagnosticCategory::Platform:
        return "Platform";
    case DiagnosticCategory::Core:
        return "Core";
    case DiagnosticCategory::Unknown:
    default:
        return "Unknown";
    }
}

} // namespace graphics::core