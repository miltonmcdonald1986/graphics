#include "internal/i_logger.hpp"

#include <memory>

#include "internal/spdlog_logger.hpp"

namespace graphics::core
{

auto get_logger() -> LoggerPtr&
{
    static LoggerPtr instance = std::make_shared<SpdlogLogger>();
    return instance;
}

} // namespace graphics::core