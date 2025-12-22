#pragma once
#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    class GraphicsContext;
    class RenderPass;

    class Renderer
    {
    public:
        static void Init(const GraphicsContext& context);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        static SDL_GPURenderPass* BeginRenderPass(const RenderPass& pass);
        static void EndRenderPass(SDL_GPURenderPass* pass);

        static SDL_GPUCommandBuffer* GetCommandBuffer();

    private:
        static SDL_GPUDevice* s_Device;
        static SDL_GPUCommandBuffer* s_CommandBuffer;
    };

}
