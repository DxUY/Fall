#pragma once

#include <string>

#include "Memory/Buffer.h"
#include "Memory/MappedFile.h"

namespace FallEngine {

	class FileSystem {
	public:
		static Buffer     ReadFileBinary(const std::string& filepath);
		static MappedFile MapFileBinary(const std::string& filepath);
	};

}
