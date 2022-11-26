#include "FileManager.h"

namespace Swag
{
	bool FileManager::WriteFile(std::string path, std::string message, unsigned int mode)
	{
		switch (mode)
		{
		case 1:
			this->_file.open(path, std::ios::out); //write
			if (this->_file.is_open())
			{
				this->_file << message << std::endl;
				this->_file.close();
				return true;
			}
			break;

		case 2:
			this->_file.open(path, std::ios::app); //write
			if (this->_file.is_open())
			{
				this->_file << message << std::endl;
				this->_file.close();
				return true;
			}
			break;
		}

		return false;
	}
	bool FileManager::ReadFile(std::string path)
	{
		return false;
	}
}