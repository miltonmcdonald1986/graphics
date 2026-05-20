#include <internal/core/spdlog_logger.hpp>

#include <string_view>

#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <graphics/core/log_level.hpp>

namespace graphics::core
{

SpdlogLogger::SpdlogLogger()
{
    m_logger = spdlog::stdout_color_mt ("graphics");

#ifdef _DEBUG
    m_logger->set_level (spdlog::level::debug);
#else
    m_logger->set_level (spdlog::level::err);
#endif

    m_logger->set_pattern ("%v");
}

auto SpdlogLogger::log (LogLevel level, std::string_view message) -> void
{
    switch (level)
    {
    case LogLevel::Trace:
        m_logger->trace (message);
        break;
    case LogLevel::Debug:
        m_logger->debug (message);
        break;
    case LogLevel::Info:
        m_logger->info (message);
        break;
    case LogLevel::Warn:
        m_logger->warn (message);
        break;
    case LogLevel::Error:
        m_logger->error (message);
        break;
    case LogLevel::Critical:
        m_logger->critical (message);
        break;
    }
}

} // namespace graphics::core