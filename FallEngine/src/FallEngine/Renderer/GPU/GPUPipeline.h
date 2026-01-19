#pragma once

#include "FallEngine/Core/Base.h"
#include "Renderer/Pipeline/PipelineHandle.h"
#include "Renderer/Pipeline/PipelineKey.h"

struct SDL_GPUGraphicsPipeline;
struct SDL_GPUComputePipeline;

namespace FallEngine {

    class GPUContext;

    class GPUPipeline {
    public:
        GPUPipeline(GPUContext& gpu);
        ~GPUPipeline() = default;

        FALL_NON_COPYABLE(GPUPipeline)

        void* CreateAndRegister(PipelineHandle handle, const PipelineKey& key);

    private:
        SDL_GPUGraphicsPipeline* CreateGraphics(const PipelineKey& key);
        SDL_GPUComputePipeline* CreateCompute(const PipelineKey& key);

        GPUContext& m_GPU;
    };

}