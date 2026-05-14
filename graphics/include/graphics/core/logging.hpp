#ifndef GRAPHICS_CORE_LOGGING_HPP
#define GRAPHICS_CORE_LOGGING_HPP

#include <string>

#include "log_level.hpp"

namespace graphics::core
{

auto log_message (LogLevel level, const std::string &message) -> void;

} // namespace graphics::core

#endif // GRAPHICS_CORE_LOGGING_HPP