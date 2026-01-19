#pragma once

#include "Renderer/Pass/RenderPass.h"
#include "Renderer/Pipeline/PipelineHandle.h"

#include "Renderer/Resource/Buffer.h"
#include "Renderer/Resource/Texture.h"

#include <vector>

namespace FallEngine {

    class Pipeline;

    struct RenderItem {
        PipelineHandle pipeline;
        Buffer vertexBuffer;
        Buffer indexBuffer;
        uint32_t elementCount;
        bool isIndexed = false;

        std::vector<Texture> vertexTextures;
        std::vector<Texture> fragmentTextures;
    };

    class ForwardRenderPass : public RenderPass {
    public:
        ForwardRenderPass() = default;

        void Submit(const RenderItem& item, const Pipeline& pipelineManager);

        virtual void Execute(GPUCommand& cmd, const FrameContext& frame) override;

    private:
        std::vector<RenderItem> m_Queue;
    };


}