#pragma once

#include "Renderer/Resource/Pipeline/PipelineKey.h"
#include <cstdint>

namespace Fall {

    class GPUBuffer;
    class GPUTexture;

    static constexpr uint32_t MAX_TEXTURE_SLOTS = 4;

    struct RenderItem {
        GPUBuffer* vertexBuffer = nullptr;
        GPUBuffer* indexBuffer = nullptr;

        GPUTexture* fragmentTextures[MAX_TEXTURE_SLOTS]{ nullptr };

        PipelineKey pipelineKey;

        uint32_t vertexBufferOffset = 0;
        uint32_t indexBufferOffset = 0;
        uint32_t elementCount = 0;
        uint32_t instanceCount = 1;
        uint32_t firstIndex = 0;
        uint32_t vertexOffset = 0;

        uint32_t textureCount = 0;
        bool indexed = false;

        uint8_t _padding[3]{ 0 };
    };
}