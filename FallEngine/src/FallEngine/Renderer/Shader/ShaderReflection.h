#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace FallEngine {

    struct ShaderBinding {
        uint32_t set;
        uint32_t binding;
        uint32_t count;
    };

    struct ShaderReflection {
        std::string entryPoint = "main";

        std::vector<ShaderBinding> sampledTextures;
        std::vector<ShaderBinding> uniformBuffers;

        std::vector<ShaderBinding> readonlyStorageTextures;
        std::vector<ShaderBinding> readwriteStorageTextures;

        std::vector<ShaderBinding> readonlyStorageBuffers;
        std::vector<ShaderBinding> readwriteStorageBuffers;
    };

}