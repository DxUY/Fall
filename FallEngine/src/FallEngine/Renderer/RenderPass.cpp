#include "FallEnginePCH.h"
#include "RenderPass.h"

namespace FallEngine {

    RenderPass::RenderPass(const RenderPassSpec& spec)
        : m_Spec(spec)
    {
        FALL_CORE_ASSERT(
            m_Spec.Color.Texture,
            "RenderPass requires a valid color target texture"
        );
    }

    SDL_GPUColorTargetInfo RenderPass::BuildColorTarget() const
    {
        SDL_GPUColorTargetInfo info{};
        info.texture = m_Spec.Color.Texture;
        info.clear_color = m_Spec.Color.ClearColor;
        info.load_op = m_Spec.Color.LoadOp;
        info.store_op = m_Spec.Color.StoreOp;
        return info;
    }

}
