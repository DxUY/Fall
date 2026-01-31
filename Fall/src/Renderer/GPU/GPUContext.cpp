#include "GPUContext.h"
#include <SDL3/SDL_gpu.h>

namespace Fall {

    bool GPUContext::Initialize(void* window) {
        m_Window = static_cast<SDL_Window*>(window);

        m_Device = SDL_CreateGPUDevice(
            SDL_GPU_SHADERFORMAT_SPIRV |
            SDL_GPU_SHADERFORMAT_DXIL |
            SDL_GPU_SHADERFORMAT_MSL,
            false,
            nullptr
        );

        if (!m_Device)
            return false;

        if (!SDL_ClaimWindowForGPUDevice(m_Device, m_Window))
            return false;

        return true;
    }

    SDL_GPUCommandBuffer* GPUContext::AcquireCommandBuffer() {
        return SDL_AcquireGPUCommandBuffer(m_Device);
    }

    void GPUContext::SubmitCommandBuffer(SDL_GPUCommandBuffer* cmd) {
        SDL_SubmitGPUCommandBuffer(cmd);
    }

    BackbufferView GPUContext::AcquireBackbuffer(SDL_GPUCommandBuffer* cmd) {
        SDL_GPUTexture* tex = nullptr;
        uint32_t w = 0, h = 0;

        if (SDL_WaitAndAcquireGPUSwapchainTexture(cmd, m_Window, &tex, &w, &h))
            return { tex, w, h };

        return {};
    }

    void GPUContext::Shutdown() {
        if (!m_Device)
            return;

        SDL_WaitForGPUIdle(m_Device);
        SDL_ReleaseWindowFromGPUDevice(m_Device, m_Window);
        SDL_DestroyGPUDevice(m_Device);

        m_Device = nullptr;
        m_Window = nullptr;
    }

}