#include "FallEnginePCH.h"
#include "ForwardRenderPass.h"

#include "Renderer/Core/FrameContext.h"
#include "Renderer/GPU/GPUCommand.h"
#include "Renderer/GPU/GPURenderPass.h"
#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPURenderTarget.h"

#include "Renderer/Target/RenderTarget.h"

#include "Renderer/Pipeline/Pipeline.h" 

namespace FallEngine {

    void ForwardRenderPass::Submit(const RenderItem& item, const Pipeline& pipelineManager) {
        FALL_ASSERT_GPU_THREAD();

        RenderItem readyItem = item;
        readyItem.pipeline = pipelineManager.GetActiveHandle(item.pipeline);

        m_Queue.push_back(readyItem);
    }

    void ForwardRenderPass::Execute(GPUCommand& cmd, const FrameContext& frame) {
        FALL_ASSERT_GPU_THREAD();

        const auto& backbufferView = frame.GetBackbuffer();
        if (!backbufferView.IsValid()) return;

        auto targetDesc = RenderTarget::Backbuffer();
        GPURenderTarget target(cmd, backbufferView, targetDesc);

        {
            GPURenderPass pass(cmd, target);

            for (const auto& item : m_Queue) {
                if (!pass.BindPipeline(item.pipeline)) {
                    FALL_CORE_WARN("Waiting for pipeline...");
                    continue;
                }

                if (!item.vertexTextures.empty()) {
                    pass.BindVertexSamplers(0, item.vertexTextures);
                }

                if (item.vertexBuffer.IsValid()) {
                    pass.BindVertexBuffers(0, { item.vertexBuffer });
                }

                if (!item.fragmentTextures.empty()) {
                    pass.BindFragmentSamplers(0, item.fragmentTextures);
                }

                if (item.isIndexed && item.indexBuffer.IsValid()) {
                    pass.BindIndexBuffer(item.indexBuffer, IndexElementSize::ThirtyTwoBit);
                    pass.DrawIndexed(item.elementCount, 1, 0, 0, 0);
                }
                else {
                    pass.Draw(item.elementCount, 1, 0, 0);
                }
            }
        }

        m_Queue.clear();
    }

}