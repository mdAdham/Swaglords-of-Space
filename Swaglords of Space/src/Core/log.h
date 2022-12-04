#pragma once

#include <spdlog/spdlog.h>

#define SWAG_DEFAULT_LOGGER_NAME "SwagLogger"

#ifdef SWAG_DEBUG
#define SWAG_TRACE(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__))
#define SWAG_INFO(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__))
#define SWAG_WARN(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__))
#define SWAG_ERROR(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__))
#define SWAG_DEBUG(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__))
#define SWAG_CRITICAL(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__))
#endif // SWAG_DEBUG

#ifdef SWAG_RELEASE
#define SWAG_TRACE(...) 
#define SWAG_INFO(...)  
#define SWAG_WARN(...)  
#define SWAG_ERROR(...) 
#define SWAG_DEBUG(...) 
#define SWAG_CRITICAL(...) 

#endif // SWAG_RELEASE

