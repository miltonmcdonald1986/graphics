#ifndef GRAPHICS_CORE_DIAGNOSTIC_HPP
#define GRAPHICS_CORE_DIAGNOSTIC_HPP

#include <source_location>
#include <string>

#include "diagnostic_category.hpp"
#include "log_level.hpp"

namespace graphics::core
{

/// Represents a single diagnostic message emitted by the engine.
///
/// A diagnostic captures categorized information about an event such as an
/// error, warning, or informational message. It includes the category,
/// severity level, source location, and a human-readable message.
struct Diagnostic
{
    /// High-level category describing the subsystem that produced
    /// the diagnostic (e.g., Core, Platform).
    DiagnosticCategory category{DiagnosticCategory::Unknown};

    /// Severity level of the diagnostic (Error, Warning, Info, etc.).
    LogLevel level{LogLevel::Error};

    /// Source location where the diagnostic originated.
    /// Automatically populated using `std::source_location::current()`.
    std::source_location location;

    /// Human-readable description of the diagnostic event.
    std::string message;
};

auto log_diagnostic (const Diagnostic& diagnostic) -> void;

auto log_diagnostic (DiagnosticCategory category,
    const std::string& message,
    LogLevel level = LogLevel::Error,
    std::source_location location = std::source_location::current()) -> void;

} // namespace graphics::core

#endif // GRAPHICS_CORE_DIAGNOSTIC_HPP