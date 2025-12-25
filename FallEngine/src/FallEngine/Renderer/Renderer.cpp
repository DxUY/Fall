#include "FallEnginePCH.h"
#include "Renderer.h"

#include "GraphicsContext.h"
#include "RenderPass.h"

namespace FallEngine {

    void Renderer::Init(const GraphicsContext& context)
    {
        s_Device = context.GetDevice();
        s_Window = context.GetWindowHandle();

        FALL_CORE_ASSERT(s_Device, "Renderer Init failed: device null");
        FALL_CORE_ASSERT(s_Window, "Renderer Init failed: window null");
    }

    void Renderer::Shutdown()
    {
        s_Device = nullptr;
    }

    void Renderer::BeginFrame()
    {
        FALL_CORE_ASSERT(!s_CommandBuffer, "Frame already begun");

        s_CommandBuffer = SDL_AcquireGPUCommandBuffer(s_Device);
        FALL_CORE_ASSERT(s_CommandBuffer, "Failed to acquire command buffer");
    }

    void Renderer::EndFrame()
    {
        FALL_CORE_ASSERT(s_CommandBuffer, "EndFrame without BeginFrame");

        SDL_SubmitGPUCommandBuffer(s_CommandBuffer);
        s_CommandBuffer = nullptr;
    }

    SDL_GPURenderPass* Renderer::BeginRenderPass(const RenderPass& pass)
    {
        FALL_CORE_ASSERT(s_CommandBuffer, "BeginRenderPass outside frame");

        SDL_Window* window = s_Window;
        SDL_GPUTexture* swapchainTexture = nullptr;

        if (!pass.GetTarget()) {
            Uint32 w = 0, h = 0;
            bool ok = SDL_WaitAndAcquireGPUSwapchainTexture(
                s_CommandBuffer, window, &swapchainTexture, &w, &h
            );
            FALL_CORE_ASSERT(ok && swapchainTexture, "Swapchain acquire failed");
        }

        SDL_GPUColorTargetInfo color = pass.BuildColorTarget(swapchainTexture);

        return SDL_BeginGPURenderPass(s_CommandBuffer, &color, 1, nullptr);
    }


    void Renderer::EndRenderPass(SDL_GPURenderPass* pass)
    {
        FALL_CORE_ASSERT(pass, "Null render pass");
        SDL_EndGPURenderPass(pass);
    }

    SDL_GPUCommandBuffer* Renderer::GetCommandBuffer()
    {
        FALL_CORE_ASSERT(s_CommandBuffer, "No active frame");
        return s_CommandBuffer;
    }

}
