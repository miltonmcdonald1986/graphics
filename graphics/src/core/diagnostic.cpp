#include <graphics/core/diagnostic.hpp>

#include <format>
#include <source_location>
#include <string>

#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/logging.hpp>

using graphics::core::DiagnosticCategory;

namespace
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

} // namespace

namespace graphics::core
{

auto log_diagnostic (const Diagnostic& diagnostic) -> void
{
    log_message (diagnostic.level,
        std::format ("[{}] {}",
            translate_diagnostic_category_to_string (diagnostic.category),
            diagnostic.message));
}

auto log_diagnostic (DiagnosticCategory category,
    const std::string& message,
    LogLevel level,
    std::source_location location) -> void
{
    const Diagnostic diagnostic{.category = category,
        .level = level,
        .location = location,
        .message = message};

    log_diagnostic (diagnostic);
}

} // namespace graphics::core