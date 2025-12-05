#pragma once

#include "Buffer.h"

namespace FallEngine {
	class FileSystem {
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}