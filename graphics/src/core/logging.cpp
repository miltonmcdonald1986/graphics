#include <graphics/core/logging.hpp>

#include <string>

#include <graphics/core/log_level.hpp>

#include <internal/core/i_logger.hpp>

using std::string;

namespace graphics::core
{

auto log_message (LogLevel level, const string& message) -> void
{
    get_logger()->log (level, message);
}

} // namespace graphics::core