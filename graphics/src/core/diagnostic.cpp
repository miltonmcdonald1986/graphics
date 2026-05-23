#include <graphics/core/diagnostic.hpp>

#include <filesystem>
#include <format>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

#include <magic_enum/magic_enum.hpp>

#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/logging.hpp>

using magic_enum::enum_name;
using std::format;
using std::move;
using std::source_location;
using std::string;
using std::string_view;
using std::filesystem::path;

namespace graphics::core
{

namespace
{

auto extract_function_name (string_view full) -> string_view
{
    // Remove argument list: "foo::bar(int)" → "foo::bar"
    if (auto pos = full.find ('('); pos != string_view::npos)
    {
        full = full.substr (0, pos);
    }

    // Remove return type: "void foo::bar" → "foo::bar"
    if (auto pos = full.rfind (' '); pos != string_view::npos)
    {
        full = full.substr (pos + 1);
    }

    return full;
}

} // namespace

auto log_diagnostic (const Diagnostic& diagnostic) -> void
{
    log_message (diagnostic.level,
        format ("Log Level: {}\n"
                "Category:  {}\n"
                "Function:  {}\n"
                "Filename:  {}\n"
                "Line:      {}\n"
                "Message:   {}\n",
            enum_name (diagnostic.level), enum_name (diagnostic.category),
            extract_function_name (diagnostic.location.function_name()),
            path (diagnostic.location.file_name()).filename().string(),
            diagnostic.location.line(), diagnostic.message));
}

auto log_diagnostic (DiagnosticCategory category,
    const string& message,
    LogLevel level,
    source_location location) -> void
{
    const Diagnostic diagnostic{.category = category,
        .level = level,
        .location = location,
        .message = message};

    log_diagnostic (diagnostic);
}

} // namespace graphics::core