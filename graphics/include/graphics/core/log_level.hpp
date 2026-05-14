#ifndef GRAPHICS_CORE_LOG_LEVEL_HPP
#define GRAPHICS_CORE_LOG_LEVEL_HPP

#include <cstdint>

namespace graphics::core
{

enum class LogLevel : std::uint8_t
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical
};

} // namespace graphics::core

#endif // GRAPHICS_CORE_LOG_LEVEL_HPP