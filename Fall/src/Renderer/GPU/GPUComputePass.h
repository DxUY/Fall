#pragma once

#include "Core/Base.h"
#include <vector>

struct SDL_GPUComputePass;
struct SDL_GPUComputePipeline;

namespace Fall {

    class GPUCommand;
    class GPUBuffer;
    class GPUTexture;

    class GPUComputePass {
    public:
        GPUComputePass(GPUCommand& cmd);
        ~GPUComputePass();

        FALL_NON_COPYABLE(GPUComputePass)

        void BindPipeline(SDL_GPUComputePipeline* pipeline);

        void BindStorageBuffers(uint32_t firstSlot, const std::vector<GPUBuffer*>& buffers);
        void BindStorageTextures(uint32_t firstSlot, const std::vector<GPUTexture*>& textures);

        void Dispatch(uint32_t x, uint32_t y, uint32_t z);

    private:
        GPUCommand& m_Cmd;
        SDL_GPUComputePass* m_Pass = nullptr;
    };
}