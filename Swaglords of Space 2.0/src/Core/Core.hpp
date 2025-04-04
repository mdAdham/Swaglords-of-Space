#pragma once

#include <memory>
#include <cassert>
#include <stdint.h>

#define _SWAG_EXPAND_MACRO(x) x
#define _SWAG_STRINGIFY_MACRO(x) #x

#define _BIT(x) (1 << x)

using Byte = unsigned char; // uint8_t
using Word = unsigned short; // uint16_t

namespace _Swag {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	static constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	static constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Log.hpp"