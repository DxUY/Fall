#pragma once

#include "Core/Base.h"

#include "Renderer/Resource/Pipeline/PipelineKey.h"
#include "Renderer/Resource/Pipeline/PipelineKeyHash.h"

#include <unordered_map>

struct SDL_GPUComputePipeline;

namespace Fall {

    class GPUContext;

    class GPUComputePipeline {
    public:
        GPUComputePipeline(GPUContext& context);
        ~GPUComputePipeline();

        FALL_NON_COPYABLE(GPUComputePipeline)

        void Create(const PipelineKey& key);
        bool HasPipeline(const PipelineKey& key) const;

        SDL_GPUComputePipeline* GetInternal(const PipelineKey& key) const;

    private:
        void ReleaseInternal();

    private:
        GPUContext& m_Context;
        std::unordered_map<PipelineKey, SDL_GPUComputePipeline*, PipelineKeyHash> m_Pipelines;
    };

}