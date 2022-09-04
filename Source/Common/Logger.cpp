#include "Precompiled.hpp"
#include "Logger.hpp"

void Logger::Setup()
{
    // Setup default loggers based on configuration
    spdlog::sinks_init_list sinks =
    {
      std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log.txt", true),
#ifdef CONFIG_DEBUG
      std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
    };

    auto logger = std::make_shared<spdlog::logger>("default", sinks);
    spdlog::set_default_logger(logger);

    // Setup log message settings based on configuration
#ifdef CONFIG_RELEASE
    spdlog::set_pattern("[%H:%M:%S][%n][%l] %v");
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_pattern("[%H:%M:%S][%n][%t][%^%l%$] %v");
    spdlog::set_level(spdlog::level::trace);
#endif
}
