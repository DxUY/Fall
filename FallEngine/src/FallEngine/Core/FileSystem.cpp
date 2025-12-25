#include "FileSystem.h"

#include <fstream>

namespace FallEngine {

	Buffer FileSystem::ReadFileBinary(const std::string& filepath) {
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
			return {};

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);

		size_t size = static_cast<size_t>(end - stream.tellg());
		if (size == 0)
			return {};

		Buffer buffer(size);
		stream.read(reinterpret_cast<char*>(buffer.Data()), size);
		return buffer;
	}

	MappedFile FileSystem::MapFileBinary(const std::string& filepath) {
		MappedFile mapped;
		mapped.Open(filepath);
		return mapped;
	}

}
