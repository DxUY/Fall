#include "FallEnginePCH.h"
#include "GPUBuffer.h"
#include "GPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    static SDL_GPUBufferUsageFlags ToSDLUsage(BufferUsage usage)
    {
        return static_cast<SDL_GPUBufferUsageFlags>(static_cast<uint32_t>(usage));
    }

    GPUBuffer::GPUBuffer(GPUContext& gpu, Buffer handle, const BufferDesc& desc)
        : m_GPU(gpu), m_Handle(handle), m_Desc(desc)
    {
        FALL_CORE_ASSERT(desc.size > 0, "GPUBuffer size must be > 0");
        FALL_CORE_ASSERT(handle.IsValid(), "Invalid Buffer handle passed to GPUBuffer");

        Create();

        m_GPU.GetBufferRegistry().Register(m_Handle, this);
    }

    GPUBuffer::~GPUBuffer()
    {
        m_GPU.GetBufferRegistry().Remove(m_Handle);

        if (m_Native) {
            SDL_ReleaseGPUBuffer(m_GPU.GetDevice(), m_Native);
            m_Native = nullptr;
        }

        if (m_PendingDestroy) {
            SDL_ReleaseGPUBuffer(m_GPU.GetDevice(), m_PendingDestroy);
            m_PendingDestroy = nullptr;
        }
    }

    void GPUBuffer::Create()
    {
        FALL_ASSERT_GPU_THREAD();

        SDL_GPUBufferCreateInfo info{};
        info.size = m_Desc.size;
        info.usage = ToSDLUsage(m_Desc.usage);

        m_Native = SDL_CreateGPUBuffer(m_GPU.GetDevice(), &info);
        FALL_CORE_ASSERT(m_Native, "Failed to create SDL_GPUBuffer");

        m_Size = m_Desc.size;
    }

    void GPUBuffer::Resize(const BufferDesc& newDesc, uint64_t currentFrame)
    {
        FALL_ASSERT_GPU_THREAD();

        if (newDesc.size <= m_Size && newDesc.usage == m_Desc.usage)
            return;

        if (m_PendingDestroy) {
            SDL_ReleaseGPUBuffer(m_GPU.GetDevice(), m_PendingDestroy);
        }

        m_PendingDestroy = m_Native;
        m_Native = nullptr;

        m_DestroyAfterFrame = currentFrame + 3; 
        m_Desc = newDesc;

        Create();
    }

    void GPUBuffer::TryDestroyDeferred(uint64_t completedFrame)
    {
        FALL_ASSERT_GPU_THREAD();

        if (m_PendingDestroy && completedFrame >= m_DestroyAfterFrame)
        {
            SDL_ReleaseGPUBuffer(m_GPU.GetDevice(), m_PendingDestroy);
            m_PendingDestroy = nullptr;
            m_DestroyAfterFrame = UINT64_MAX;
        }
    }
}