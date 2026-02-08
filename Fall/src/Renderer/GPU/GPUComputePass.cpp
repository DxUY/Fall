#include "FallPCH.h"
#include "GPUComputePass.h"

#include "GPUCommand.h"
#include "GPUBuffer.h"
#include "GPUTexture.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPUComputePass::GPUComputePass(GPUCommand& cmd) : m_Cmd(cmd) {
        FALL_ASSERT_GPU_THREAD();
        m_Pass = SDL_BeginGPUComputePass(cmd.GetNative(), nullptr, 0, nullptr, 0);
        FALL_CORE_ASSERT(m_Pass, "Failed to begin Compute Pass");
    }

    void GPUComputePass::BindPipeline(SDL_GPUComputePipeline* pipeline) {
        if (pipeline) SDL_BindGPUComputePipeline(m_Pass, pipeline);
    }

    void GPUComputePass::BindStorageBuffers(uint32_t firstSlot, GPUBuffer** buffers, uint32_t count) {
        if (count == 0 || !buffers) return;

        uint32_t actualCount = (count > MAX_COMPUTE_BINDINGS) ? MAX_COMPUTE_BINDINGS : count;
        SDL_GPUBuffer* natives[MAX_COMPUTE_BINDINGS];

        for (uint32_t i = 0; i < actualCount; ++i) {
            natives[i] = buffers[i] ? buffers[i]->GetNative() : nullptr;
        }

        SDL_BindGPUComputeStorageBuffers(m_Pass, firstSlot, natives, actualCount);
    }

    void GPUComputePass::BindStorageTextures(uint32_t firstSlot, GPUTexture** textures, uint32_t count) {
        if (count == 0 || !textures) return;

        uint32_t actualCount = (count > MAX_COMPUTE_BINDINGS) ? MAX_COMPUTE_BINDINGS : count;
        SDL_GPUTexture* natives[MAX_COMPUTE_BINDINGS];

        for (uint32_t i = 0; i < actualCount; ++i) {
            natives[i] = textures[i] ? textures[i]->GetNative() : nullptr;
        }

        SDL_BindGPUComputeStorageTextures(m_Pass, firstSlot, natives, actualCount);
    }

    void GPUComputePass::Dispatch(uint32_t x, uint32_t y, uint32_t z) {
        SDL_DispatchGPUCompute(m_Pass, x, y, z);
    }

    GPUComputePass::~GPUComputePass() {
        if (m_Pass) SDL_EndGPUComputePass(m_Pass);
    }

}