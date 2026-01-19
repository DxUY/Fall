#include "FallEnginePCH.h"
#include "GPUContext.h"
#include "GPUTexture.h"
#include "GPUBuffer.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    GPUContext::~GPUContext() {
#ifdef FALL_DEBUG
        FALL_CORE_ASSERT(m_Device == nullptr, "GPUContext destroyed without Shutdown()");
#endif
    }

    bool GPUContext::Initialize(void* window) {
        SetGPUThreadID();
        FALL_ASSERT_GPU_THREAD();

        m_Window = static_cast<SDL_Window*>(window);

        m_Device = SDL_CreateGPUDevice(
            SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
            false,
            nullptr
        );

        if (!m_Device) {
            FALL_CORE_ERROR("Failed to create SDL_GPUDevice: {0}", SDL_GetError());
            return false;
        }

        if (!SDL_ClaimWindowForGPUDevice(m_Device, m_Window)) {
            FALL_CORE_ERROR("Failed to claim window for GPU device: {0}", SDL_GetError());
            return false;
        }

        SDL_GPUTextureFormat swapchainFormat = SDL_GetGPUSwapchainTextureFormat(m_Device, m_Window);
        FALL_CORE_INFO("Swapchain Format: {0}", (uint32_t)swapchainFormat);

        return true;
    }

    BackbufferView GPUContext::AcquireBackbuffer(SDL_GPUCommandBuffer* cmd) {
        FALL_ASSERT_GPU_THREAD();
        SDL_GPUTexture* swapchainTex = nullptr;
        uint32_t w, h;

        if (SDL_WaitAndAcquireGPUSwapchainTexture(cmd, m_Window, &swapchainTex, &w, &h)) {
            return { static_cast<void*>(swapchainTex), w, h };
        }

        return { nullptr, 0, 0 };
    }

    void GPUContext::Shutdown() {
        FALL_ASSERT_GPU_THREAD();

        if (m_Device) {
            SDL_WaitForGPUIdle(m_Device);

            auto texMap = m_TextureRegistry.ExtractAll();
            for (auto it = texMap.begin(); it != texMap.end(); ++it) {
                GPUTexture* tex = it->second;
                delete tex;
            }
            texMap.clear();

            auto buffMap = m_BufferRegistry.ExtractAll();
            for (auto it = buffMap.begin(); it != buffMap.end(); ++it) {
                GPUBuffer* buff = it->second;
                delete buff;
            }
            buffMap.clear();

            auto gfxMap = m_GraphicsPipelines.ExtractAll();
            for (auto& [id, ptr] : gfxMap) {
                SDL_ReleaseGPUGraphicsPipeline(m_Device, ptr);
            }
            gfxMap.clear();

            auto compMap = m_ComputePipelines.ExtractAll();
            for (auto& [id, ptr] : compMap) {
                SDL_ReleaseGPUComputePipeline(m_Device, ptr);
            }
            compMap.clear();

            SDL_ReleaseWindowFromGPUDevice(m_Device, m_Window);
            SDL_DestroyGPUDevice(m_Device);

            m_Device = nullptr;
        }
        m_Window = nullptr;
    }

    SDL_GPUCommandBuffer* GPUContext::AcquireCommandBuffer() {
        return SDL_AcquireGPUCommandBuffer(m_Device);
    }

    void GPUContext::SubmitCommandBuffer(SDL_GPUCommandBuffer* cmd) {
        SDL_SubmitGPUCommandBuffer(cmd);
    }

}