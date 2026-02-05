#pragma once

#include "Renderer/Pass/Pass.h"
#include "RenderItem.h"

#include <vector>

namespace Fall {

    class PipelineManager; 

    class ForwardRenderPass final : public Pass {
    public:
        ForwardRenderPass(PipelineManager& pipelineMgr);

        void Submit(const RenderItem& item);
        void Execute(GPUCommand& cmd, const FrameContext& frame) override;

    private:
        PipelineManager& m_PipelineMgr;
        std::vector<RenderItem> m_Queue;
    };

}