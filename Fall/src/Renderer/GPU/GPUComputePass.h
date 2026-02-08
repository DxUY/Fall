#pragma once

#include "Core/Base.h"
#include <cstdint>

struct SDL_GPUComputePass;
struct SDL_GPUComputePipeline;

namespace Fall {

    class GPUCommand;
    class GPUBuffer;
    class GPUTexture;

    constexpr uint32_t MAX_COMPUTE_BINDINGS = 8;

    class GPUComputePass {
    public:
        GPUComputePass(GPUCommand& cmd);
        ~GPUComputePass();

        FALL_NON_COPYABLE(GPUComputePass)

        void BindPipeline(SDL_GPUComputePipeline* pipeline);

        void BindStorageBuffers(uint32_t firstSlot, GPUBuffer** buffers, uint32_t count);
        void BindStorageTextures(uint32_t firstSlot, GPUTexture** textures, uint32_t count);

        void Dispatch(uint32_t x, uint32_t y, uint32_t z);

    private:
        GPUCommand& m_Cmd;
        SDL_GPUComputePass* m_Pass = nullptr;
    };

}