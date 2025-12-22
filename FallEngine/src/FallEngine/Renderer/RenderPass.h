#pragma once
#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    struct ColorAttachment
    {
        SDL_GPUTexture* Texture = nullptr;

        SDL_FColor ClearColor = { 0, 0, 0, 1 };
        SDL_GPULoadOp LoadOp = SDL_GPU_LOADOP_CLEAR;
        SDL_GPUStoreOp StoreOp = SDL_GPU_STOREOP_STORE;
    };

    struct RenderPassSpec
    {
        ColorAttachment Color;
    };

    class RenderPass
    {
    public:
        RenderPass(const RenderPassSpec& spec);

        SDL_GPUColorTargetInfo BuildColorTarget() const;

    private:
        RenderPassSpec m_Spec;
    };

}
