#include "FallPCH.h"
#include "GPUCommand.h"
#include "GPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    struct GPUCommand::Impl {
        SDL_GPUCommandBuffer* Cmd = nullptr;
        bool Recording = false;
    };

    GPUCommand::GPUCommand(GPUContext& gpu)
        : m_Impl(CreateScope<Impl>()), m_GPU(gpu) {
    }

    GPUCommand::~GPUCommand() {
#ifdef FALL_DEBUG
        FALL_CORE_ASSERT(!m_Impl->Recording,
            "GPUCommand destroyed while still recording");
#endif
    }

    void GPUCommand::Begin() {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(!m_Impl->Recording,
            "GPUCommand::Begin() called while already recording");

        m_Impl->Cmd = m_GPU.AcquireCommandBuffer();

        if (!m_Impl->Cmd) {
            const char* error = SDL_GetError();
            FALL_CORE_ERROR("SDL_AcquireGPUCommandBuffer failed! Reason: {0}", error);
            m_Impl->Recording = false;
            return;
        }

        m_Impl->Recording = true;
    }

    void GPUCommand::End() {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(m_Impl->Recording,
            "GPUCommand::End() called without Begin()");

        if (m_Impl->Cmd) {
            m_GPU.SubmitCommandBuffer(m_Impl->Cmd);
        }
        else {
            FALL_CORE_ERROR("Attempting to submit a null GPU command buffer");
        }

        m_Impl->Cmd = nullptr;
        m_Impl->Recording = false;
    }

    bool GPUCommand::IsRecording() const {
        return m_Impl->Recording;
    }

    SDL_GPUCommandBuffer* GPUCommand::GetNative() const {
        FALL_CORE_ASSERT(m_Impl->Cmd,
            "GPUCommand::GetNative() called while not recording");
        return m_Impl->Cmd;
    }

    void GPUCommand::PushVertexUniform(uint32_t slot, const void* data, uint32_t size) {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(m_Impl->Recording,
            "PushVertexUniform called outside of recording");
        FALL_CORE_ASSERT(data && size > 0,
            "Invalid vertex uniform data");

        SDL_PushGPUVertexUniformData(m_Impl->Cmd, slot, data, size);
    }

    void GPUCommand::PushFragmentUniform(uint32_t slot, const void* data, uint32_t size) {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(m_Impl->Recording,
            "PushFragmentUniform called outside of recording");
        FALL_CORE_ASSERT(data && size > 0,
            "Invalid fragment uniform data");

        SDL_PushGPUFragmentUniformData(m_Impl->Cmd, slot, data, size);
    }

    void GPUCommand::PushComputeUniform(uint32_t slot, const void* data, uint32_t size) {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(m_Impl->Recording,
            "PushComputeUniform called outside of recording");
        FALL_CORE_ASSERT(data && size > 0,
            "Invalid compute uniform data");

        SDL_PushGPUComputeUniformData(m_Impl->Cmd, slot, data, size);
    }

}