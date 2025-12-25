#include "FallEnginePCH.h"

#include "RenderPass.h"
#include "RenderTarget.h"

#include "Texture.h"

#include "Pipeline.h"

namespace FallEngine {

    RenderPass::RenderPass(const RenderPassSpec& spec)
        : m_Spec(spec)
    {
        if (m_Spec.Target)
            FALL_CORE_ASSERT(m_Spec.Target->IsValid(), "Invalid RenderTarget");
    }

    const RenderTarget* RenderPass::GetTarget() const
    {
        return m_Spec.Target;
    }

    RenderPassSignature RenderPass::GetSignature() const
    {
        RenderPassSignature sig{};

        // Currently your engine supports exactly one color target
        sig.colorCount = 1;

        if (m_Spec.Target)
        {
            // ---------- Offscreen RenderTarget ----------
            const Texture* color = m_Spec.Target->Color;
            FALL_CORE_ASSERT(color, "RenderTarget missing color texture");

            const TextureSpec& colorSpec = color->GetSpec();

            sig.colorFormats[0] = colorSpec.Format;
            sig.sampleCount = 1; // You are not exposing MSAA yet

            if (m_Spec.Target->Depth)
            {
                sig.depthFormat =
                    m_Spec.Target->Depth->GetSpec().Format;
            }
            else
            {
                sig.depthFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
            }
        }
        else
        {
            // ---------- Swapchain ----------
            sig.colorFormats[0] =
                SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;

            sig.depthFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
            sig.sampleCount = 1;
        }

        return sig;
    }

    SDL_GPUColorTargetInfo RenderPass::BuildColorTarget(SDL_GPUTexture* swapchainTexture) const
    {
        SDL_GPUColorTargetInfo info{};
        if (m_Spec.Target) {
            // Offscreen target
            FALL_CORE_ASSERT(m_Spec.Target->Color, "Offscreen RenderTarget missing color texture");
            info.texture = m_Spec.Target->Color->GetHandle();
        }
        else {
            // Swapchain
            FALL_CORE_ASSERT(swapchainTexture, "Swapchain texture is null");
            info.texture = swapchainTexture;
        }

        info.clear_color = m_Spec.Color.ClearColor;
        info.load_op = m_Spec.Color.LoadOp;
        info.store_op = m_Spec.Color.StoreOp;

        return info;
    }

}
