#pragma once

#include "Renderer/Resource/Buffer/BufferDesc.h"
#include "Core/Base.h"

struct SDL_GPUTransferBuffer;

namespace Fall {

    class GPUContext;
    class GPUCommand;
    class GPUBuffer;

    class GPURingBuffer {
    public:
        static Scope<GPURingBuffer> CreateUniformBuffer(GPUContext& context, uint32_t totalSize);
        static Scope<GPURingBuffer> CreateComputeBuffer(GPUContext& context, uint32_t totalSize);

        GPURingBuffer(GPUContext& context, uint32_t totalSize, BufferUsage usage);
        ~GPURingBuffer();

        FALL_NON_COPYABLE(GPURingBuffer)

        uint32_t Push(GPUCommand& cmd, const void* data, uint32_t size);
        void Reset() { m_CurrentOffset = 0; }

        GPUBuffer& GetInternalBuffer() { return *m_BackingBuffer; }

    private:
        GPUContext& m_Context;
        Scope<GPUBuffer> m_BackingBuffer;

        SDL_GPUTransferBuffer* m_TransferBuffer = nullptr;
        uint8_t* m_MappedData = nullptr;

        uint32_t m_CurrentOffset = 0;
        uint32_t m_TotalSize = 0;

        static constexpr uint32_t ALIGNMENT = 256;
    };

}