#include "FallPCH.h"
#include "GPUComputePass.h"

#include "GPUCommand.h"
#include "GPUBuffer.h"
#include "GPUTexture.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPUComputePass::GPUComputePass(GPUCommand& cmd) : m_Cmd(cmd) {
        m_Pass = SDL_BeginGPUComputePass(cmd.GetNative(), nullptr, 0, nullptr, 0);
        FALL_CORE_ASSERT(m_Pass, "Failed to begin Compute Pass");
    }

    void GPUComputePass::BindPipeline(SDL_GPUComputePipeline* pipeline) {
        if (pipeline) SDL_BindGPUComputePipeline(m_Pass, pipeline);
    }

    void GPUComputePass::BindStorageBuffers(uint32_t firstSlot, const std::vector<GPUBuffer*>& buffers) {
        std::vector<SDL_GPUBuffer*> natives;
        natives.reserve(buffers.size());

        for (auto* buf : buffers) {
            if (buf) {
                natives.push_back(buf->GetNative());
            }
        }

        if (!natives.empty()) {
            SDL_BindGPUComputeStorageBuffers(
                m_Pass,
                firstSlot,
                natives.data(),
                static_cast<uint32_t>(natives.size())
            );
        }
    }

    void GPUComputePass::BindStorageTextures(uint32_t firstSlot, const std::vector<GPUTexture*>& textures) {
        std::vector<SDL_GPUTexture*> natives;
        natives.reserve(textures.size());

        for (auto* tex : textures) {
            if (tex) {
                natives.push_back(tex->GetNative());
            }
        }

        if (!natives.empty()) {
            SDL_BindGPUComputeStorageTextures(
                m_Pass,
                firstSlot,
                natives.data(),
                static_cast<uint32_t>(natives.size())
            );
        }
    }

    void GPUComputePass::Dispatch(uint32_t x, uint32_t y, uint32_t z) {
        SDL_DispatchGPUCompute(m_Pass, x, y, z);
    }

    GPUComputePass::~GPUComputePass() {
        if (m_Pass) SDL_EndGPUComputePass(m_Pass);
    }
}