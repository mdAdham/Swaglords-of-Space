#pragma once

#include <filesystem>
#include "Core/Buffer.hpp"

namespace _Swag {
	struct _MetaData
	{
		std::filesystem::path _metadatapath;
		Buffer data;

		void Load();
	};
}