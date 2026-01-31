#include "FallPCH.h"
#include "GPURenderPass.h"

#include "GPUCommand.h"
#include "GPURenderTarget.h"
#include "GPUBuffer.h"
#include "GPUTexture.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPURenderPass::GPURenderPass(GPUCommand& cmd, const GPURenderTarget& target)
        : m_Cmd(cmd) {
        FALL_ASSERT_GPU_THREAD();
        const auto* colorInfo = static_cast<const SDL_GPUColorTargetInfo*>(target.GetNativeInfo());

        // Assuming 1 color target for now based on your previous code
        m_Pass = SDL_BeginGPURenderPass(cmd.GetNative(), colorInfo, 1, nullptr);
        FALL_CORE_ASSERT(m_Pass, "Failed to begin GPU render pass: {0}", SDL_GetError());
    }

    void GPURenderPass::BindPipeline(SDL_GPUGraphicsPipeline* pipeline) {
        if (pipeline) {
            SDL_BindGPUGraphicsPipeline(m_Pass, pipeline);
        }
    }

    void GPURenderPass::BindVertexBuffers(uint32_t firstBinding, const std::vector<GPUBuffer*>& buffers) {
        std::vector<SDL_GPUBufferBinding> bindings;
        bindings.reserve(buffers.size());

        for (auto* buffer : buffers) {
            if (buffer && buffer->GetNative()) {
                bindings.push_back(SDL_GPUBufferBinding{ buffer->GetNative(), 0 });
            }
        }

        if (!bindings.empty()) {
            SDL_BindGPUVertexBuffers(m_Pass, firstBinding, bindings.data(), static_cast<uint32_t>(bindings.size()));
        }
    }

    void GPURenderPass::BindIndexBuffer(GPUBuffer* buffer, IndexElementSize indexSize) {
        if (buffer && buffer->GetNative()) {
            SDL_GPUIndexElementSize size = (indexSize == IndexElementSize::ThirtyTwoBit)
                ? SDL_GPU_INDEXELEMENTSIZE_32BIT : SDL_GPU_INDEXELEMENTSIZE_16BIT;

            SDL_GPUBufferBinding binding = { buffer->GetNative(), 0 };
            SDL_BindGPUIndexBuffer(m_Pass, &binding, size);
        }
    }

    void GPURenderPass::BindVertexSamplers(uint32_t firstSlot, const std::vector<GPUTexture*>& textures) {
        std::vector<SDL_GPUTextureSamplerBinding> bindings;
        bindings.reserve(textures.size());

        for (auto* tex : textures) {
            if (tex && tex->GetNative()) {
                bindings.push_back(SDL_GPUTextureSamplerBinding{ tex->GetNative(), nullptr });
            }
        }

        if (!bindings.empty()) {
            SDL_BindGPUVertexSamplers(m_Pass, firstSlot, bindings.data(), static_cast<uint32_t>(bindings.size()));
        }
    }

    void GPURenderPass::BindFragmentSamplers(uint32_t firstSlot, const std::vector<GPUTexture*>& textures) {
        std::vector<SDL_GPUTextureSamplerBinding> bindings;
        bindings.reserve(textures.size());

        for (auto* tex : textures) {
            if (tex && tex->GetNative()) {
                bindings.push_back(SDL_GPUTextureSamplerBinding{ tex->GetNative(), nullptr });
            }
        }

        if (!bindings.empty()) {
            SDL_BindGPUFragmentSamplers(m_Pass, firstSlot, bindings.data(), static_cast<uint32_t>(bindings.size()));
        }
    }

    void GPURenderPass::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
        SDL_DrawGPUPrimitives(m_Pass, vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void GPURenderPass::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance) {
        SDL_DrawGPUIndexedPrimitives(m_Pass, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    GPURenderPass::~GPURenderPass() {
        if (m_Pass) SDL_EndGPURenderPass(m_Pass);
    }

}