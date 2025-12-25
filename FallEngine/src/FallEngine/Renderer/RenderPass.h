#pragma once

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    struct RenderTarget;
	struct RenderPassSignature;

    struct ColorAttachmentSpec {
        SDL_FColor     ClearColor{ 0, 0, 0, 1 };
        SDL_GPULoadOp  LoadOp = SDL_GPU_LOADOP_CLEAR;
        SDL_GPUStoreOp StoreOp = SDL_GPU_STOREOP_STORE;
    };

    struct RenderPassSpec {
        const RenderTarget* Target = nullptr;
        ColorAttachmentSpec Color;
    };

    class RenderPass {
    public:
        RenderPass(const RenderPassSpec& spec);

        SDL_GPUColorTargetInfo BuildColorTarget(SDL_GPUTexture* swapchainTexture) const;

        const RenderTarget* GetTarget() const;

        RenderPassSignature GetSignature() const;

    private:
        RenderPassSpec m_Spec;
    };

}
