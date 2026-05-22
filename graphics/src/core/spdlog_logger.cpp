#include <internal/core/spdlog_logger.hpp>

#include <string_view>

#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <graphics/core/log_level.hpp>

using spdlog::get;
#ifdef _DEBUG
using spdlog::level::debug;
#else
using spdlog::level::err;
#endif
using spdlog::level::level_enum;
using spdlog::stdout_color_mt;
using std::string_view;

namespace graphics::core
{

SpdlogLogger::SpdlogLogger()
{
    constexpr const char* logger_name = "graphics";
    if (auto existing = get (logger_name))
        m_logger = existing;
    else
        m_logger = stdout_color_mt (logger_name);

#ifdef _DEBUG
    m_logger->set_level (debug);
#else
    m_logger->set_level (err);
#endif

    m_logger->set_pattern ("%v");
}

auto SpdlogLogger::log (LogLevel level, string_view message) -> void
{
    m_logger->log (static_cast<level_enum> (level), message);
}

} // namespace graphics::core