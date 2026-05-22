#ifndef GRAPHICS_CORE_EXPECTED_HPP
#define GRAPHICS_CORE_EXPECTED_HPP

#include <expected>

#include "diagnostic.hpp"

namespace graphics::core
{

#pragma region Aliases

/// \brief Convenience alias for expected results using engine diagnostics.
///
/// The `Expected<T>` alias represents an operation that may either produce
/// a value of type `T` or a `Diagnostic` describing the failure. This is the
/// standard return type for functions that participate in the engine's
/// error-handling and diagnostic system.
///
/// \tparam T The value type returned on success.
template <typename T> using Expected = std::expected<T, Diagnostic>;

/// \brief Convenience alias for unexpected diagnostic results.
///
/// The `Unexpected` alias represents an error outcome containing a
/// `Diagnostic`. It is used together with `Expected<T>` to express
/// operations that may fail and return detailed diagnostic information.
///
/// This type is returned when an operation cannot produce a value and must
/// instead report a failure condition.
///
/// \see Expected
using Unexpected = std::unexpected<Diagnostic>;

#pragma endregion

#pragma region functions

/// \brief Creates an `Unexpected` value from an existing diagnostic.
///
/// Wraps the given `Diagnostic` inside an `Unexpected` object. This is the
/// preferred way to propagate a previously constructed diagnostic through
/// functions that return `Expected<T>` or `Status`.
///
/// \param diagnostic The diagnostic describing the failure.
/// \return An `Unexpected` containing the provided diagnostic.
///
/// \see Unexpected
/// \see Expected
auto create_unexpected (const Diagnostic& diagnostic) -> Unexpected;

/// \brief Creates an `Unexpected` value from diagnostic components.
///
/// Constructs a `Diagnostic` using the provided category, message, severity
/// level, and source location, then wraps it inside an `Unexpected`. This
/// overload is convenient for inline error reporting where a full
/// `Diagnostic` object is not already available.
///
/// \param category The subsystem category associated with the diagnostic.
/// \param message Human-readable description of the failure.
/// \param level Severity level of the diagnostic. Defaults to `Error`.
/// \param location Source location where the diagnostic originated.
///                 Defaults to `std::source_location::current()`.
/// \return An `Unexpected` containing the constructed diagnostic.
///
/// \see Unexpected
/// \see Diagnostic
/// \see Expected
auto create_unexpected (DiagnosticCategory category,
    std::string message,
    LogLevel level = LogLevel::Error,
    std::source_location location = std::source_location::current())
    -> Unexpected;

#pragma endregion

} // namespace graphics::core

#endif // GRAPHICS_CORE_EXPECTED_HPP