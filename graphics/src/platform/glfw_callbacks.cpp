#include "glfw_callbacks.hpp"

#include <format>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/logging.hpp>

namespace graphics::platform
{

    auto glfw_error_callback(int error_code, const char* description) -> void
    {
        core::log_message(core::LogLevel::Error, core::create_diagnostic_message(
            core::DiagnosticCategory::Platform, 
            std::format(
                "GLFW error {}: {}", 
                error_code, 
                description
            )
        ));
    }

}