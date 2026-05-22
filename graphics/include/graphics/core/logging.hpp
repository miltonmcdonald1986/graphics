#ifndef GRAPHICS_CORE_LOGGING_HPP
#define GRAPHICS_CORE_LOGGING_HPP

#include <string>

#include "log_level.hpp"

namespace graphics::core
{

/// \brief Emits a formatted log message at the given severity level.
///
/// Sends the provided message to the engine's logging backend using the
/// specified `LogLevel`. This is the lowest-level logging entry point and is
/// used internally by higher-level diagnostic helpers such as
/// `log_diagnostic()`.
///
/// \param level Severity level of the message.
/// \param message Human-readable text to be logged.
auto log_message (LogLevel level, std::string message) -> void;

} // namespace graphics::core

#endif // GRAPHICS_CORE_LOGGING_HPP