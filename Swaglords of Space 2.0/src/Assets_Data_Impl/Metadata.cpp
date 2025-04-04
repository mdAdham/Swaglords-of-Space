#include "Metadata.hpp"
#include <fstream>

#include "Core/Log.hpp"

namespace _Swag {

	void _Swag::_MetaData::Load()
	{
		if (this->_metadatapath.extension().string() == "metadata")
		{
			std::ifstream file(_metadatapath, std::ios::binary | std::ios::ate);
			if (!file)
			{
				_SWAG_ERROR("Failed to open Metadata file! '{0}'", _metadatapath.string());
			}
			
			std::streampos end = file.tellg();
			file.seekg(0, std::ios::beg);
			uint64_t size = end - file.tellg();
			if (size == 0)
			{
				_SWAG_ERROR("MetaData file is Empty! '{0}'", _metadatapath.string());
			}
			file.read(data.As<char>(), size);
			file.close();
			_SWAG_TRACE("Sucessfully loaded Metadata file! '{0}'", _metadatapath.string());
		}
		else
		{
			_SWAG_ERROR("Invalid Metadata file! '{0}'", this->_metadatapath.string());
		}
	}
}