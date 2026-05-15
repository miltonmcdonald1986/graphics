#ifndef GRAPHICS_CORE_I_LOGGER_HPP
#define GRAPHICS_CORE_I_LOGGER_HPP

#include <memory>
#include <string_view>

#include <graphics/core/log_level.hpp>

namespace graphics::core
{

struct ILogger
{
    virtual ~ILogger() = default;
    virtual auto log (LogLevel level, std::string_view message) -> void = 0;
};

using LoggerPtr = std::shared_ptr<ILogger>;
auto get_logger() -> LoggerPtr&;

}

#endif // GRAPHICS_CORE_I_LOGGER_HPP