#include <graphics/core/expected.hpp>

#include <expected>
#include <source_location>
#include <string>
#include <utility>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/log_level.hpp>

using std::move;
using std::source_location;
using std::string;
using std::unexpected;

namespace graphics::core
{

auto create_unexpected (const Diagnostic& diagnostic) -> Unexpected
{
    log_diagnostic (diagnostic);
    return unexpected (diagnostic);
}

auto create_unexpected (DiagnosticCategory category,
    string message,
    LogLevel level,
    source_location location) -> Unexpected
{
    const Diagnostic diagnostic{.category = category,
        .level = level,
        .location = location,
        .message = move (message)};

    return create_unexpected (diagnostic);
}

} // namespace graphics::core