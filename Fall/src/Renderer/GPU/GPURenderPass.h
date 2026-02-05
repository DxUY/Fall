#pragma once

#include "Core/Base.h"
#include <vector>

struct SDL_GPURenderPass;
struct SDL_GPUGraphicsPipeline;

namespace Fall {

    class GPUCommand;
    class GPURenderTarget;
    class GPUBuffer;
    class GPUTexture;

    enum class IndexElementSize { SixteenBit, ThirtyTwoBit };

    class GPURenderPass {
    public:
        GPURenderPass(GPUCommand& cmd, const GPURenderTarget& target);
        ~GPURenderPass();

        FALL_NON_COPYABLE(GPURenderPass)

        void BindPipeline(SDL_GPUGraphicsPipeline* pipeline);

        void BindVertexBuffers(uint32_t firstBinding, const std::vector<GPUBuffer*>& buffers);
        void BindIndexBuffer(GPUBuffer* buffer, IndexElementSize indexSize);

        void BindVertexSamplers(uint32_t firstSlot, const std::vector<GPUTexture*>& textures);
        void BindFragmentSamplers(uint32_t firstSlot, const std::vector<GPUTexture*>& textures);

        void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
        void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0, uint32_t vertexOffset = 0, uint32_t firstInstance = 0);

        SDL_GPURenderPass* GetNative() const { return m_Pass; }

    private:
        GPUCommand& m_Cmd;
        SDL_GPURenderPass* m_Pass = nullptr;
    };

}