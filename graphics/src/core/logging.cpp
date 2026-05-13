#include <graphics/core/logging.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace graphics::core
{

	auto log_message(
		LogLevel level, 
		const std::string& message
	) -> void
	{
		static bool first_time = true;
		static auto sp_logger = spdlog::stdout_color_mt("graphics");
		if (first_time)
		{
#ifdef _DEBUG
			sp_logger->set_level(spdlog::level::debug);
#else
			sp_logger->set_level(spdlog::level::err);
#endif
		}

		first_time = false;
		
		switch (level)
		{
		case graphics::core::LogLevel::Trace:
			sp_logger->trace(message);
			break;
		case graphics::core::LogLevel::Debug:
		default:
			sp_logger->debug(message);
			break;
		case graphics::core::LogLevel::Info:
			sp_logger->info(message);
			break;
		case graphics::core::LogLevel::Warn:
			sp_logger->warn(message);
			break;
		case graphics::core::LogLevel::Error:
			sp_logger->error(message);
			break;
		case graphics::core::LogLevel::Critical:
			sp_logger->critical(message);
			break;
		}
	}

} // namespace graphics::core