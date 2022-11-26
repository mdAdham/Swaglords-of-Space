#pragma once
#include <string>
#include <fstream>

namespace Swag
{
	class FileManager
	{
	public:
		FileManager() { }

		bool WriteFile(std::string path, std::string message, unsigned int mode);
		bool ReadFile(std::string path);

	private:
		std::fstream _file;
	};
}