#include "FallPCH.h"
#include "GPURingBuffer.h"

#include "GPUContext.h"
#include "GPUCommand.h"
#include "GPUBuffer.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    Scope<GPURingBuffer> GPURingBuffer::CreateUniformBuffer(GPUContext& context, uint32_t totalSize) {
        return CreateScope<GPURingBuffer>(context, totalSize, BufferUsage::GraphicsRead);
    }

    Scope<GPURingBuffer> GPURingBuffer::CreateComputeBuffer(GPUContext& context, uint32_t totalSize) {
        return CreateScope<GPURingBuffer>(context, totalSize, BufferUsage::ComputeRead | BufferUsage::ComputeWrite);
    }

    GPURingBuffer::GPURingBuffer(GPUContext& context, uint32_t totalSize, BufferUsage usage)
        : m_Context(context), m_TotalSize(totalSize) {

        BufferDesc desc{};
        desc.size = totalSize;
        desc.usage = usage;
        m_BackingBuffer = CreateScope<GPUBuffer>(m_Context, desc);

        SDL_GPUTransferBufferCreateInfo xferInfo{};
        xferInfo.size = totalSize;
        xferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        m_TransferBuffer = SDL_CreateGPUTransferBuffer(m_Context.GetDevice(), &xferInfo);

        m_MappedData = (uint8_t*)SDL_MapGPUTransferBuffer(m_Context.GetDevice(), m_TransferBuffer, false);
    }

    GPURingBuffer::~GPURingBuffer() {
        if (m_TransferBuffer) {
            SDL_UnmapGPUTransferBuffer(m_Context.GetDevice(), m_TransferBuffer);
            SDL_ReleaseGPUTransferBuffer(m_Context.GetDevice(), m_TransferBuffer);
        }
    }

    uint32_t GPURingBuffer::Push(GPUCommand& cmd, const void* data, uint32_t size) {
        FALL_ASSERT_GPU_THREAD();

        m_CurrentOffset = (m_CurrentOffset + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
        FALL_ASSERT(m_CurrentOffset + size <= m_TotalSize, "Ring Buffer Overflow!");

        uint32_t writeOffset = m_CurrentOffset;

        memcpy(m_MappedData + writeOffset, data, size);

        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd.GetNative());
        SDL_GPUTransferBufferLocation src{ m_TransferBuffer, writeOffset };
        SDL_GPUBufferRegion dst{ m_BackingBuffer->GetNative(), writeOffset, size };

        SDL_UploadToGPUBuffer(copyPass, &src, &dst, false);
        SDL_EndGPUCopyPass(copyPass);

        m_CurrentOffset += size;
        return writeOffset;
    }

}