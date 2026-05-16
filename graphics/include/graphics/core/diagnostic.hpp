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
    /// High-level category describing the subsystem that produced the
    /// diagnostic (e.g., Core, Platform).
    DiagnosticCategory category{DiagnosticCategory::Unknown};

    /// Severity level of the diagnostic (Error, Warning, Info, etc.).
    LogLevel level{LogLevel::Error};

    /// Source location where the diagnostic originated.

    /// Normally, this would be automatically populated using
    /// `std::source_location::current()`.
    std::source_location location;

    /// Human-readable description of the diagnostic event.
    std::string message;
};

/// \brief Logs a fully constructed diagnostic message.
///
/// Emits the given `Diagnostic` to the engine's logging backend. This
/// overload is used when the caller has already assembled a complete
/// diagnostic, including category, level, source location, and message.
///
/// \param diagnostic The diagnostic object to log.
auto log_diagnostic (const Diagnostic& diagnostic) -> void;

/// \brief Logs a diagnostic message using explicit components.
///
/// Constructs a `Diagnostic` from the provided category, message, severity
/// level, and source location, then emits it to the logging backend. This
/// overload is convenient for inline error reporting where a full
/// `Diagnostic` object is not already available.
///
/// \param category The subsystem category associated with the diagnostic.
/// \param message Human-readable description of the diagnostic event.
/// \param level Severity level of the diagnostic. Defaults to `Error`.
/// \param location Source location where the diagnostic originated.
///                 Defaults to `std::source_location::current()`.
auto log_diagnostic (DiagnosticCategory category,
    const std::string& message,
    LogLevel level = LogLevel::Error,
    std::source_location location = std::source_location::current()) -> void;

} // namespace graphics::core

#endif // GRAPHICS_CORE_DIAGNOSTIC_HPP