#pragma once

#include "FallEngine/Core/Base.h"
#include "Renderer/Pipeline/PipelineHandle.h"
#include "Renderer/Resource/Buffer.h"
#include "Renderer/Resource/Texture.h"

struct SDL_GPURenderPass;

namespace FallEngine {

    class GPUCommand;
    class GPURenderTarget;

    enum class IndexElementSize { SixteenBit, ThirtyTwoBit };

    class GPURenderPass {
    public:
        GPURenderPass(GPUCommand& cmd, const GPURenderTarget& target);
        ~GPURenderPass();

        FALL_NON_COPYABLE(GPURenderPass)

        bool BindPipeline(PipelineHandle handle);

        void BindVertexBuffers(uint32_t firstBinding, const std::vector<Buffer>& buffers);
        void BindIndexBuffer(Buffer buffer, IndexElementSize indexSize);

        void BindVertexSamplers(uint32_t firstSlot, const std::vector<Texture>& textures);
        void BindFragmentSamplers(uint32_t firstSlot, const std::vector<Texture>& textures);

        void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0, uint32_t vertexOffset = 0, uint32_t firstInstance = 0);

    private:
        GPUCommand& m_Cmd;
        SDL_GPURenderPass* m_Pass = nullptr;
    };

}