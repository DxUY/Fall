#include "FallEnginePCH.h"
#include "GPURenderPass.h"

#include "GPUCommand.h"
#include "GPUContext.h"
#include "GPURenderTarget.h"
#include "GPUBuffer.h"
#include "GPUTexture.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    template<typename THandle, typename TNative, typename Binding>
    static void BuildTextureSamplerBindings(GPUContext& ctx, const std::vector<THandle>& inputs, std::vector<Binding>& out, std::function<TNative* (THandle)> resolve) {
        out.clear();
        out.reserve(inputs.size());
        for (const auto& handle : inputs) {
            if (auto* native = resolve(handle)) {
                out.push_back(Binding{ native, nullptr });
            }
        }
    }

    GPURenderPass::GPURenderPass(GPUCommand& cmd, const GPURenderTarget& target)
        : m_Cmd(cmd) {
        FALL_ASSERT_GPU_THREAD();
        const auto* colorInfo = static_cast<const SDL_GPUColorTargetInfo*>(target.GetNativeInfo());

        m_Pass = SDL_BeginGPURenderPass(cmd.GetNative(), colorInfo, 1, nullptr);
        FALL_CORE_ASSERT(m_Pass, "Failed to begin GPU render pass: {0}", SDL_GetError());
    }

    bool GPURenderPass::BindPipeline(PipelineHandle handle) {
        FALL_ASSERT_GPU_THREAD();

        auto* native = m_Cmd.GetGPUContext().GetGraphicsPipelineRegistry().Resolve(handle);
        if (native) {
            SDL_BindGPUGraphicsPipeline(m_Pass, native);
            return true; 
        }

        return false; 
    }

    void GPURenderPass::BindVertexBuffers(uint32_t firstBinding, const std::vector<Buffer>& buffers) {
        std::vector<SDL_GPUBufferBinding> bindings;
        bindings.reserve(buffers.size());

        for (const auto& handle : buffers) {
            auto* gpuBuf = m_Cmd.GetGPUContext().GetBufferRegistry().Resolve(handle);
            if (gpuBuf) {
                bindings.push_back(SDL_GPUBufferBinding{ gpuBuf->GetNative(), 0 });
            }
        }

        if (!bindings.empty()) {
            SDL_BindGPUVertexBuffers(m_Pass, firstBinding, bindings.data(), static_cast<uint32_t>(bindings.size()));
        }
    }

    void GPURenderPass::BindIndexBuffer(Buffer buffer, IndexElementSize indexSize) {
        auto* gpuBuf = m_Cmd.GetGPUContext().GetBufferRegistry().Resolve(buffer);
        if (gpuBuf) {
            SDL_GPUIndexElementSize size = (indexSize == IndexElementSize::ThirtyTwoBit)
                ? SDL_GPU_INDEXELEMENTSIZE_32BIT : SDL_GPU_INDEXELEMENTSIZE_16BIT;

            SDL_GPUBufferBinding binding = { gpuBuf->GetNative(), 0 };
            SDL_BindGPUIndexBuffer(m_Pass, &binding, size);
        }
    }

    void GPURenderPass::BindVertexSamplers(uint32_t firstSlot, const std::vector<Texture>& textures) {
        FALL_ASSERT_GPU_THREAD();

        std::vector<SDL_GPUTextureSamplerBinding> bindings;

        BuildTextureSamplerBindings<Texture, SDL_GPUTexture, SDL_GPUTextureSamplerBinding>(m_Cmd.GetGPUContext(),
                textures,
                bindings,
                [&](Texture handle) {
                    if (auto* tex =
                        m_Cmd.GetGPUContext()
                        .GetTextureRegistry()
                        .Resolve(handle)) {
                        return tex->GetNative();
                    }
                    return static_cast<SDL_GPUTexture*>(nullptr);
                });

        if (!bindings.empty()) {
            SDL_BindGPUVertexSamplers(m_Pass, firstSlot, bindings.data(), static_cast<uint32_t>(bindings.size()));
        }
    }

    void GPURenderPass::BindFragmentSamplers(uint32_t firstSlot, const std::vector<Texture>& textures) {
        FALL_ASSERT_GPU_THREAD();

        std::vector<SDL_GPUTextureSamplerBinding> bindings;

        BuildTextureSamplerBindings<Texture, SDL_GPUTexture, SDL_GPUTextureSamplerBinding>(m_Cmd.GetGPUContext(),
                textures,
                bindings,
                [&](Texture handle) {
                    if (auto* tex =
                        m_Cmd.GetGPUContext()
                        .GetTextureRegistry()
                        .Resolve(handle)) {
                        return tex->GetNative();
                    }
                    return static_cast<SDL_GPUTexture*>(nullptr);
                });

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