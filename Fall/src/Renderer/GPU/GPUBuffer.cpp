#include "FallPCH.h"
#include "GPUBuffer.h"

#include "GPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    static SDL_GPUBufferUsageFlags ToSDLUsage(BufferUsage usage) {
        return static_cast<SDL_GPUBufferUsageFlags>(static_cast<uint32_t>(usage));
    }

    GPUBuffer::GPUBuffer(GPUContext& context, const BufferDesc& desc)
        : m_Context(context), m_Desc(desc) {
        FALL_ASSERT_GPU_THREAD();
        Create();
    }

    GPUBuffer::~GPUBuffer() {
        FALL_ASSERT_GPU_THREAD();
        ReleaseInternal();
    }

    SDL_GPUBuffer* GPUBuffer::GetNative() const {
        FALL_ASSERT(m_Native);
        return m_Native;
    }

    void GPUBuffer::Create() {
        FALL_ASSERT_GPU_THREAD();
        FALL_ASSERT(m_Desc.size > 0);

        SDL_GPUBufferCreateInfo info{};
        info.size = m_Desc.size;
        info.usage = ToSDLUsage(m_Desc.usage);

        m_Native = SDL_CreateGPUBuffer(m_Context.GetDevice(), &info);
        FALL_ASSERT(m_Native, "Failed to create GPU buffer");
    }

    void GPUBuffer::ReleaseInternal() {
        if (m_Native) {
            SDL_GPUDevice* device = m_Context.GetDevice();
            SDL_GPUBuffer* buffer = m_Native;

            m_Context.EnqueueDeletion([device, buffer]() {
                SDL_ReleaseGPUBuffer(device, buffer);
                });

            m_Native = nullptr;
        }
    }

    void GPUBuffer::Resize(const BufferDesc& newDesc) {
        FALL_ASSERT_GPU_THREAD();

        if (newDesc.size <= m_Desc.size && newDesc.usage == m_Desc.usage) return;

        ReleaseInternal();

        m_Desc = newDesc;
        Create();
    }

}