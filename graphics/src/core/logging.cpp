#include <graphics/core/logging.hpp> // NOLINT(misc-include-cleaner)

#include <string>

#include <graphics/core/log_level.hpp>

#include <graphics_internal/core/i_logger.hpp>

namespace graphics::core
{

auto log_message (LogLevel level, const std::string& message) -> void
{
    get_logger()->log (level, message);
}

} // namespace graphics::core