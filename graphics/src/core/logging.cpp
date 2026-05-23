#include <graphics/core/logging.hpp>

#include <string>
#include <utility>

#include <graphics/core/log_level.hpp>

#include <internal/core/i_logger.hpp>

using std::move;
using std::string;

namespace graphics::core
{

auto log_message (LogLevel level, string message) -> void
{
    get_logger()->log (level, move (message));
}

} // namespace graphics::core