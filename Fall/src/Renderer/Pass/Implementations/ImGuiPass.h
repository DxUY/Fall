#pragma once

#include "Renderer/Pass/Pass.h"

namespace Fall {

    class PipelineManager;

    class ImGuiPass final : public Pass {
    public:
        void Execute(GPUCommand& cmd, const FrameContext& frame) override;
    };

}