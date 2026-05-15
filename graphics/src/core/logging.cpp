#include <graphics/core/logging.hpp> // NOLINT(misc-include-cleaner)

#include <memory>
#include <string>

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <graphics/core/log_level.hpp>

#include <graphics_internal/core/i_logger.hpp>

namespace graphics::core
{

auto log_message (LogLevel level, const std::string &message) -> void
{
    get_logger()->log (level, message);
}

} // namespace graphics::core