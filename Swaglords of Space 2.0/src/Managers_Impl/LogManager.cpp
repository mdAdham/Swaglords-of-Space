#include "LogManager.hpp"
#include "Core/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <vector>

namespace _Swag {
	void LogManager::Initialize()
	{
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_pattern("%^[%T] %n: %v%$");

		std::vector<spdlog::sink_ptr> sinks{ consoleSink };
		auto logger = std::make_shared<spdlog::logger>(SWAG_DEFAULT_LOGGER_NAME, sinks.begin(), sinks.end());

		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);

		spdlog::register_logger(logger);
	}

	void LogManager::ShutDown()
	{
		spdlog::shutdown();
	}
}