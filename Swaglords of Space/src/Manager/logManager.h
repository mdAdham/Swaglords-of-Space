#pragma once

namespace Swag
{
	class LogManager
	{
	public:
		LogManager() = default;
		~LogManager() = default;

		static void Initialize();
		static void ShutDown();
	};
}