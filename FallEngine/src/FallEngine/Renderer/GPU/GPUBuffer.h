#pragma once

#include <cstdint>
#include "FallEngine/Core/Base.h"
#include "Renderer/Resource/Buffer.h"
#include "Renderer/Resource/BufferDesc.h"

struct SDL_GPUBuffer;

namespace FallEngine {

    class GPUContext;

    class GPUBuffer {
    public:
        GPUBuffer(GPUContext& gpu, Buffer handle, const BufferDesc& desc);
        ~GPUBuffer();

        FALL_NON_COPYABLE(GPUBuffer)

        SDL_GPUBuffer* GetNative() const { return m_Native; }
        Buffer GetHandle() const { return m_Handle; }

        void Resize(const BufferDesc& newDesc, uint64_t currentFrame);
        void TryDestroyDeferred(uint64_t completedFrame);

    private:
        void Create();
        void ReleaseNative(SDL_GPUBuffer* buffer);

    private:
        GPUContext& m_GPU;
        BufferDesc  m_Desc{};

        SDL_GPUBuffer* m_Native = nullptr;
        SDL_GPUBuffer* m_PendingDestroy = nullptr;

        uint64_t m_Size = 0;
        uint64_t m_DestroyAfterFrame = UINT64_MAX;

        Buffer m_Handle{};
    };

}