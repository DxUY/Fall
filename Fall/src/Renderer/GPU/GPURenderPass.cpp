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

        m_Pass = SDL_BeginGPURenderPass(cmd.GetNative(), colorInfo, 1, nullptr);
        FALL_CORE_ASSERT(m_Pass, "Failed to begin GPU render pass");
    }

    void GPURenderPass::BindPipeline(SDL_GPUGraphicsPipeline* pipeline) {
        if (pipeline) SDL_BindGPUGraphicsPipeline(m_Pass, pipeline);
    }

    void GPURenderPass::BindVertexBuffers(uint32_t firstBinding, const std::vector<GPUBuffer*>& buffers, const std::vector<uint32_t>& offsets) {
        std::vector<SDL_GPUBufferBinding> bindings;
        bindings.reserve(buffers.size());

        for (size_t i = 0; i < buffers.size(); ++i) {
            if (buffers[i] && buffers[i]->GetNative()) {
                uint32_t offset = (i < offsets.size()) ? offsets[i] : 0;
                bindings.push_back({ buffers[i]->GetNative(), offset });
            }
        }

        if (!bindings.empty()) {
            SDL_BindGPUVertexBuffers(m_Pass, firstBinding, bindings.data(), (uint32_t)bindings.size());
        }
    }

    void GPURenderPass::BindIndexBuffer(GPUBuffer* buffer, uint32_t offset, IndexElementSize indexSize) {
        if (buffer && buffer->GetNative()) {
            SDL_GPUIndexElementSize size = (indexSize == IndexElementSize::ThirtyTwoBit)
                ? SDL_GPU_INDEXELEMENTSIZE_32BIT : SDL_GPU_INDEXELEMENTSIZE_16BIT;

            SDL_GPUBufferBinding binding = { buffer->GetNative(), offset };
            SDL_BindGPUIndexBuffer(m_Pass, &binding, size);
        }
    }

    void GPURenderPass::BindVertexSamplers(uint32_t firstSlot, GPUTexture** textures, uint32_t count) {
        if (count == 0 || !textures) return;
        SDL_GPUTextureSamplerBinding b[8];
        uint32_t c = (count > 8) ? 8 : count;
        for (uint32_t i = 0; i < c; ++i) {
            b[i] = { textures[i] ? textures[i]->GetNative() : nullptr, nullptr };
        }
        SDL_BindGPUVertexSamplers(m_Pass, firstSlot, b, c);
    }

    void GPURenderPass::BindFragmentSamplers(uint32_t firstSlot, GPUTexture** textures, uint32_t count) {
        if (count == 0 || !textures) return;
        SDL_GPUTextureSamplerBinding b[8];
        uint32_t c = (count > 8) ? 8 : count;
        for (uint32_t i = 0; i < c; ++i) {
            b[i] = { textures[i] ? textures[i]->GetNative() : nullptr, nullptr };
        }
        SDL_BindGPUFragmentSamplers(m_Pass, firstSlot, b, c);
    }

    void GPURenderPass::Draw(uint32_t vCount, uint32_t iCount, uint32_t fV, uint32_t fI) {
        SDL_DrawGPUPrimitives(m_Pass, vCount, iCount, fV, fI);
    }

    void GPURenderPass::DrawIndexed(uint32_t idxCount, uint32_t instCount, uint32_t fIdx, uint32_t vOff, uint32_t fInst) {
        SDL_DrawGPUIndexedPrimitives(m_Pass, idxCount, instCount, fIdx, vOff, fInst);
    }

    GPURenderPass::~GPURenderPass() {
        if (m_Pass) SDL_EndGPURenderPass(m_Pass);
    }

}