#include "internal/glfw_callbacks.hpp"

#include <format>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>

namespace graphics::platform
{

auto glfw_error_callback (int error_code, const char* description) -> void
{
    core::log_diagnostic (core::DiagnosticCategory::Platform,
        std::format ("GLFW error {}: {}", error_code, description));
}

} // namespace graphics::platform