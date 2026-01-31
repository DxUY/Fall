#pragma once

#include "Renderer/Resource/Pipeline/PipelineKey.h"

#include <vector>
#include <cstdint>

namespace Fall {

    class GPUBuffer;
    class GPUTexture;

    struct RenderItem {
        PipelineKey pipelineKey;

        GPUBuffer* vertexBuffer = nullptr;
        GPUBuffer* indexBuffer = nullptr;

        uint32_t elementCount = 0;
        uint32_t instanceCount = 1;
        uint32_t firstIndex = 0;
        uint32_t vertexOffset = 0;

        bool indexed = false;

        // Resource Bindings
        std::vector<GPUTexture*> vertexTextures;
        std::vector<GPUTexture*> fragmentTextures;
    };

}