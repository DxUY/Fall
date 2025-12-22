#pragma once

#include <filesystem>

#include "Memory/Buffer.h"
#include "Memory/MappedFile.h"

namespace FallEngine {

	class FileSystem {
	public:
		static Buffer     ReadFileBinary(const std::filesystem::path& filepath);
		static MappedFile MapFileBinary(const std::filesystem::path& filepath);
	};

}
