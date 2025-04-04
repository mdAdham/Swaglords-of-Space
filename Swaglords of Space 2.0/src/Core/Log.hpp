#pragma once

#include <spdlog/spdlog.h>

constexpr auto SWAG_DEFAULT_LOGGER_NAME = "SwagLogger";

#ifdef SWAG_DEBUG
#define _SWAG_TRACE(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__))
#define _SWAG_INFO(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__))
#define _SWAG_WARN(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__))
#define _SWAG_ERROR(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__))
#define _SWAG_DEBUGS(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__))
#define _SWAG_CRITICAL(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__))
#endif // SWAG_DEBUG

#ifdef SWAG_RELEASE
#define _SWAG_TRACE(...) 
#define _SWAG_INFO(...) 
#define _SWAG_WARN(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__))
#define _SWAG_ERROR(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__))
#define _SWAG_DEBUGS(...) 
#define _SWAG_CRITICAL(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__))
#endif // SWAG_DEBUG

#ifdef SWAG_DIST
#define _SWAG_TRACE(...) 
#define _SWAG_INFO(...) 
#define _SWAG_WARN(...) 
#define _SWAG_ERROR(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__))
#define _SWAG_DEBUGS(...) 
#define _SWAG_CRITICAL(...) if(spdlog::get(SWAG_DEFAULT_LOGGER_NAME) != nullptr) (spdlog::get(SWAG_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__))
#endif // SWAG_DEBUG
