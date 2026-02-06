#include "FallPCH.h"

#include "GPUCommand.h"
#include "GPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    struct GPUCommand::Impl {
        SDL_GPUCommandBuffer* Cmd = nullptr;
        SDL_GPUFence* LastFence = nullptr; 
        bool Recording = false;
    };

    GPUCommand::GPUCommand(GPUContext& gpu)
        : m_Impl(CreateScope<Impl>()), m_GPU(gpu) {
    }

    GPUCommand::~GPUCommand() {
        if (m_Impl->LastFence) {
            SDL_ReleaseGPUFence(m_GPU.GetDevice(), m_Impl->LastFence);
        }
    }

    void GPUCommand::Begin() {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(!m_Impl->Recording, "GPUCommand::Begin() called while already recording");

        if (m_Impl->LastFence) {
            SDL_ReleaseGPUFence(m_GPU.GetDevice(), m_Impl->LastFence);
            m_Impl->LastFence = nullptr;
        }

        m_Impl->Cmd = m_GPU.AcquireCommandBuffer();
        if (!m_Impl->Cmd) {
            FALL_CORE_ERROR("SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());
            return;
        }

        m_Impl->Recording = true;
    }

    void GPUCommand::End() {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(m_Impl->Recording, "GPUCommand::End() called without Begin()");

        if (m_Impl->Cmd) {
            m_Impl->LastFence = SDL_SubmitGPUCommandBufferAndAcquireFence(m_Impl->Cmd);
        }

        m_Impl->Cmd = nullptr;
        m_Impl->Recording = false;
    }

    SDL_GPUFence* GPUCommand::GetLastFence() const {
        return m_Impl->LastFence;
    }

    bool GPUCommand::IsRecording() const { return m_Impl->Recording; }

    SDL_GPUCommandBuffer* GPUCommand::GetNative() const {
        FALL_CORE_ASSERT(m_Impl->Cmd, "GPUCommand::GetNative() called while not recording");
        return m_Impl->Cmd;
    }

    void GPUCommand::PushVertexUniform(uint32_t slot, const void* data, uint32_t size) {
        SDL_PushGPUVertexUniformData(m_Impl->Cmd, slot, data, size);
    }

    void GPUCommand::PushFragmentUniform(uint32_t slot, const void* data, uint32_t size) {
        SDL_PushGPUFragmentUniformData(m_Impl->Cmd, slot, data, size);
    }

    void GPUCommand::PushComputeUniform(uint32_t slot, const void* data, uint32_t size) {
        SDL_PushGPUComputeUniformData(m_Impl->Cmd, slot, data, size);
    }
}