#ifndef GRAPHICS_CORE_LOG_LEVEL_HPP
#define GRAPHICS_CORE_LOG_LEVEL_HPP

#include <cstdint>

namespace graphics::core
{

/// \brief Severity level of a diagnostic message.
///
/// `LogLevel` defines the importance or urgency of a diagnostic emitted by
/// the engine. These levels correspond directly to the logging backend and
/// allow filtering, routing, and structured reporting.
///
/// - `Trace` - Extremely fine-grained information, typically only useful
///   during deep debugging.
/// - `Debug` - General debugging information.
/// - `Info` - High-level informational messages about normal operation.
/// - `Warn` - Something unexpected occurred, but the engine can continue.
/// - `Error` - A failure occurred that prevents an operation from succeeding.
/// - `Critical` - A severe failure that may compromise engine stability.
///
/// \see Diagnostic
/// \see DiagnosticCategory
enum class LogLevel : std::uint8_t
{
    Trace,   ///< Fine-grained debugging information.
    Debug,   ///< General debugging information.
    Info,    ///< Informational messages.
    Warn,    ///< Recoverable issues or unexpected states.
    Error,   ///< Non-recoverable operation failure.
    Critical ///< Severe failure requiring immediate attention.
};

} // namespace graphics::core

#endif // GRAPHICS_CORE_LOG_LEVEL_HPP