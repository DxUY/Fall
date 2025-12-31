#include "FallEnginePCH.h"
#include "FileSystem.h"
#include <fstream>

namespace FallEngine {

    Buffer FileSystem::ReadFileBinary(const std::string& filepath) {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream.is_open()) {
            return {};
        }

        const std::streamsize size = stream.tellg();
        if (size <= 0) {
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