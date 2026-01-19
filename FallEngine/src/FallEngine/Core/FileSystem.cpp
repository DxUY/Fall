#include "FallEnginePCH.h"
#include "FileSystem.h"
#include <fstream>

namespace FallEngine {

    Buffer FileSystem::ReadFileBinary(const std::string& filepath) {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream.is_open()) {
            return {};
        }

        std::streampos pos = stream.tellg();
        if (pos == std::streampos(-1)) {
            return {};
        }

        const std::streamoff size = static_cast<std::streamoff>(pos);
        if (size <= 0) {
            return {};
        }

        if (static_cast<unsigned long long>(size) > static_cast<unsigned long long>(std::numeric_limits<size_t>::max())) {
            FALL_CORE_ERROR("File too large to read into memory: {0}", filepath);
            return {};
        }

        stream.seekg(0, std::ios::beg);

        Buffer buffer(static_cast<size_t>(size));
        if (!stream.read(reinterpret_cast<char*>(buffer.Data()), size)) {
            return {};
        }

        return buffer;
    }

    MappedFile FileSystem::MapFileBinary(const std::string& filepath) {
        MappedFile mapped;
        mapped.Open(filepath); 
        return mapped;
    }

}