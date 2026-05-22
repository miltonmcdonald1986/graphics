#include <internal/core/i_logger.hpp>

#include <memory>

#include <internal/core/spdlog_logger.hpp>

using std::make_shared;

namespace graphics::core
{

auto get_logger() -> LoggerPtr&
{
    static LoggerPtr instance = make_shared<SpdlogLogger>();
    return instance;
}

} // namespace graphics::core