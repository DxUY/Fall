#include "FallPCH.h"
#include "ForwardRenderPass.h"

#include "Renderer/Core/FrameContext.h"
#include "Renderer/GPU/GPUCommand.h"
#include "Renderer/GPU/GPURenderPass.h"
#include "Renderer/GPU/GPURenderTarget.h"
#include "Renderer/GPU/GPUGraphicsPipeline.h" 

#include "Renderer/Resource/Pipeline/PipelineManager.h"
#include "Renderer/Resource/Target/RenderTarget.h"

namespace Fall {

    ForwardRenderPass::ForwardRenderPass(PipelineManager& pipelineMgr)
        : m_PipelineMgr(pipelineMgr) {
    }

    void ForwardRenderPass::Submit(const RenderItem& item) {
        FALL_ASSERT_GPU_THREAD();
        m_Queue.push_back(item);
    }

    void ForwardRenderPass::Execute(GPUCommand& cmd, const FrameContext& frame) {
        FALL_ASSERT_GPU_THREAD();

        if (m_Queue.empty()) return;

        std::sort(m_Queue.begin(), m_Queue.end(), [](const RenderItem& a, const RenderItem& b) {
            if (!(a.pipelineKey == b.pipelineKey)) {
                return a.pipelineKey < b.pipelineKey;
            }

            auto* texA = (a.textureCount > 0) ? a.fragmentTextures[0] : nullptr;
            auto* texB = (b.textureCount > 0) ? b.fragmentTextures[0] : nullptr;
            return texA < texB;
            });

        const auto& backbuffer = frame.GetBackbuffer();
        if (!backbuffer.IsValid()) return;

        GPURenderTarget target(backbuffer, RenderTarget::Backbuffer());
        {
            GPURenderPass pass(cmd, target);

            const PipelineKey* lastKey = nullptr;
            GPUTexture* lastTexture = nullptr;

            for (const RenderItem& item : m_Queue) {
                if (!lastKey || !(item.pipelineKey == *lastKey)) {
                    auto* nativePipeline = m_PipelineMgr.GetGraphicsCache().GetInternal(item.pipelineKey);
                    if (nativePipeline) {
                        pass.BindPipeline(nativePipeline);
                        lastKey = &item.pipelineKey;
                    }
                    else continue;
                }

                if (item.textureCount > 0) {
                    if (item.fragmentTextures[0] != lastTexture) {
                        pass.BindFragmentSamplers(0, (GPUTexture**)item.fragmentTextures, item.textureCount);
                        lastTexture = item.fragmentTextures[0];
                    }
                }

                if (item.vertexBuffer) {
                    pass.BindVertexBuffers(0, { item.vertexBuffer }, { item.vertexBufferOffset });
                }

                if (item.indexed && item.indexBuffer) {
                    pass.BindIndexBuffer(item.indexBuffer, item.indexBufferOffset, IndexElementSize::ThirtyTwoBit);
                    pass.DrawIndexed(item.elementCount, item.instanceCount, item.firstIndex, item.vertexOffset);
                }
                else {
                    pass.Draw(item.elementCount, item.instanceCount);
                }
            }
        }
        m_Queue.clear();
    }

}