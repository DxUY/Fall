#pragma once

#include "Core/Base.h"
#include <cstdint>

struct SDL_GPUCommandBuffer;

namespace Fall {

    class GPUContext;

    class GPUCommand {
    public:
        GPUCommand(GPUContext& gpu);
        ~GPUCommand();

        FALL_NON_COPYABLE(GPUCommand)

        void Begin();
        void End();

        bool IsRecording() const;

        void PushVertexUniform(uint32_t slot, const void* data, uint32_t size);
        void PushFragmentUniform(uint32_t slot, const void* data, uint32_t size);
        void PushComputeUniform(uint32_t slot, const void* data, uint32_t size);

        SDL_GPUCommandBuffer* GetNative() const;

    private:
        struct Impl;
        Scope<Impl> m_Impl;
        GPUContext& m_GPU;
    };

}