#include "FallEnginePCH.h"
#include "Renderer.h"
#include "GraphicsContext.h"
#include "RenderPass.h"

namespace FallEngine {

    SDL_GPUDevice* Renderer::s_Device = nullptr;
    SDL_GPUCommandBuffer* Renderer::s_CommandBuffer = nullptr;

    void Renderer::Init(const GraphicsContext& context)
    {
        s_Device = context.GetDevice();
        FALL_CORE_ASSERT(s_Device, "Renderer Init failed: device null");
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

        SDL_GPUColorTargetInfo color = pass.BuildColorTarget();

        return SDL_BeginGPURenderPass(
            s_CommandBuffer,
            &color,
            1,
            nullptr
        );
    }

    void Renderer::EndRenderPass(SDL_GPURenderPass* pass)
    {
        SDL_EndGPURenderPass(pass);
    }

    SDL_GPUCommandBuffer* Renderer::GetCommandBuffer()
    {
        FALL_CORE_ASSERT(s_CommandBuffer, "No active frame");
        return s_CommandBuffer;
    }

}