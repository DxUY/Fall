#include "FallPCH.h"
#include "ClearPass.h"

#include "Renderer/Core/FrameContext.h"

#include "Renderer/GPU/GPUCommand.h"
#include "Renderer/GPU/GPURenderPass.h"
#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPURenderTarget.h"

#include "Renderer/Resource/Target/RenderTarget.h"

namespace Fall {
    void ClearPass::Execute(GPUCommand& cmd, const FrameContext& frame) {
        BackbufferView backbuffer = frame.GetBackbuffer();

        if (!backbuffer.IsValid()) return;

        ClearColor clear = { m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a };
        RenderTarget descriptor = RenderTarget::Backbuffer(clear);

        GPURenderTarget target(backbuffer, descriptor);
        {
            GPURenderPass pass(cmd, target);
        } 
    }

}