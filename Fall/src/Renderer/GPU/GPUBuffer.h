#pragma once

#include "Renderer/Resource/Buffer/BufferDesc.h"
#include "Core/Base.h"

struct SDL_GPUBuffer;

namespace Fall {

    class GPUContext;

    class GPUBuffer {
    public:
        GPUBuffer(GPUContext& gpu, const BufferDesc& desc);
        ~GPUBuffer();

        FALL_NON_COPYABLE(GPUBuffer)

        SDL_GPUBuffer* GetNative() const;

        const BufferDesc& GetDesc() const { return m_Desc; }

        void Resize(const BufferDesc& newDesc);

    private:
        void Create();

    private:
        GPUContext& m_GPU;
        BufferDesc m_Desc{};
        SDL_GPUBuffer* m_Native = nullptr;
    };

}