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

        BufferDesc desc{ totalSize, usage };
        m_BackingBuffer = CreateScope<GPUBuffer>(m_Context, desc);

        SDL_GPUTransferBufferCreateInfo xferInfo{};
        xferInfo.size = totalSize;
        xferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;

        m_TransferBuffer = SDL_CreateGPUTransferBuffer(m_Context.GetDevice(), &xferInfo);
        FALL_CORE_ASSERT(m_TransferBuffer, "Failed to create Ring Buffer Transfer hardware!");

        m_MappedData = (uint8_t*)SDL_MapGPUTransferBuffer(m_Context.GetDevice(), m_TransferBuffer, false);
        FALL_CORE_ASSERT(m_MappedData, "Failed to map Ring Buffer Transfer memory!");
    }

    GPURingBuffer::~GPURingBuffer() {
        if (m_TransferBuffer) {
            if (m_MappedData) {
                SDL_UnmapGPUTransferBuffer(m_Context.GetDevice(), m_TransferBuffer);
            }
            SDL_ReleaseGPUTransferBuffer(m_Context.GetDevice(), m_TransferBuffer);
        }
    }

    uint32_t GPURingBuffer::Push(GPUCommand& cmd, const void* data, uint32_t size) {
        FALL_ASSERT_GPU_THREAD();

        if (!m_MappedData || size == 0 || !data) {
            if (!data && size > 0) {
                FALL_CORE_ERROR("GPURingBuffer::Push: Null data pointer provided!");
            }
            return UINT32_MAX;
        }

        uint32_t alignedOffset = (m_CurrentOffset + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

        if (alignedOffset + size > m_TotalSize) {
            FALL_CORE_ERROR("GPURingBuffer Overflow! Increase total size ({0} bytes requested)", alignedOffset + size);
            return UINT32_MAX;
        }

        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd.GetNative());
        if (!copyPass) {
            FALL_CORE_ERROR("GPURingBuffer::Push: Failed to begin GPU copy pass: {0}", SDL_GetError());
            return UINT32_MAX;
        }

        uint32_t writeOffset = alignedOffset;
        memcpy(m_MappedData + writeOffset, data, size);

        SDL_GPUTransferBufferLocation src{ m_TransferBuffer, writeOffset };
        SDL_GPUBufferRegion dst{ m_BackingBuffer->GetNative(), writeOffset, size };

        SDL_UploadToGPUBuffer(copyPass, &src, &dst, false);
        SDL_EndGPUCopyPass(copyPass);

        m_CurrentOffset = writeOffset + size;

        return writeOffset;
    }

}