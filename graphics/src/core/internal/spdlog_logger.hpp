#ifndef GRAPHICS_CORE_SPDLOG_LOGGER_HPP
#define GRAPHICS_CORE_SPDLOG_LOGGER_HPP

#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <graphics/core/log_level.hpp>

#include "i_logger.hpp"

namespace graphics::core
{

class SpdlogLogger final : public ILogger
{
  public:
    SpdlogLogger();
    ~SpdlogLogger() override = default;

    auto log (LogLevel level, std::string_view message) -> void override;

  private:
    std::shared_ptr<spdlog::logger> m_logger;
};

} // namespace graphics::core

#endif // GRAPHICS_CORE_SPDLOG_LOGGER_HPP