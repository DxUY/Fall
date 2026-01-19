#include "ClearPass.h"

#include "Renderer/Core/FrameContext.h"

#include "Renderer/GPU/GPUCommand.h"
#include "Renderer/GPU/GPURenderPass.h"
#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPURenderTarget.h"

#include "Renderer/Target/RenderTarget.h"

namespace FallEngine {

    void ClearPass::Execute(GPUCommand& cmd, const FrameContext& frame) {
        BackbufferView backbuffer = frame.GetBackbuffer();

        if (!backbuffer.opaque) return;

        ClearColor clear = { m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a };
        RenderTarget descriptor = RenderTarget::Backbuffer(clear);

        GPURenderTarget target(cmd, backbuffer, descriptor);
        {
            GPURenderPass pass(cmd, target); 
        } 
    }

}