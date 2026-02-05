#pragma once

#include "PipelineKey.h"
#include "PipelineKeyHash.h"

#include <unordered_set>

namespace Fall {

    class GPUContext;
    class GPUGraphicsPipeline;
    class GPUComputePipeline;

    class PipelineManager {
    public:
        explicit PipelineManager(GPUContext& context);
        ~PipelineManager();

        FALL_NON_COPYABLE(PipelineManager)

        void CreateGraphicsPipeline(const PipelineKey& key);
        void CreateComputePipeline(const PipelineKey& key);

        bool HasGraphicsPipeline(const PipelineKey& key) const;
        bool HasComputePipeline(const PipelineKey& key) const;

        GPUGraphicsPipeline& GetGraphicsCache() { return *m_Graphics; }
        GPUComputePipeline& GetComputeCache() { return *m_Compute; }

    private:
        GPUContext& m_Context;

        Scope<GPUGraphicsPipeline> m_Graphics;
        Scope<GPUComputePipeline>  m_Compute;

        std::unordered_set<PipelineKey, PipelineKeyHash> m_GraphicsKeys;
        std::unordered_set<PipelineKey, PipelineKeyHash> m_ComputeKeys;
    };

}