#include <internal/platform/glfw_callbacks.hpp>

#include <format>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>

using graphics::core::log_diagnostic;
using graphics::core::DiagnosticCategory::Platform;
using std::format;

namespace graphics::platform
{

auto glfw_error_callback (int error_code, const char* description) -> void
{
    log_diagnostic (Platform,
        format ("GLFW error {}: {}", error_code, description));
}

} // namespace graphics::platform