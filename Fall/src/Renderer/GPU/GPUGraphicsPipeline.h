#pragma once

#include "Core/Base.h"

#include "Renderer/Resource/Pipeline/PipelineKey.h"
#include "Renderer/Resource/Pipeline/PipelineKeyHash.h"

#include <unordered_map>

struct SDL_GPUGraphicsPipeline;

namespace Fall {

    class GPUContext;

    class GPUGraphicsPipeline {
    public:
        GPUGraphicsPipeline(GPUContext& context);
        ~GPUGraphicsPipeline();

        FALL_NON_COPYABLE(GPUGraphicsPipeline)

        void Create(const PipelineKey& key);
        bool HasPipeline(const PipelineKey& key) const;

        SDL_GPUGraphicsPipeline* GetInternal(const PipelineKey& key) const;

    private:
        GPUContext& m_Context;

        std::unordered_map<PipelineKey, SDL_GPUGraphicsPipeline*, PipelineKeyHash> m_Pipelines;
    };

}