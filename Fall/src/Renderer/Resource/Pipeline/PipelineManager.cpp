#include "FallPCH.h"
#include "PipelineManager.h"

#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPUGraphicsPipeline.h"
#include "Renderer/GPU/GPUComputePipeline.h"

namespace Fall {

    PipelineManager::PipelineManager(GPUContext& gpu)
        : m_GPU(gpu) {

        FALL_ASSERT_GPU_THREAD();

        m_Graphics = CreateScope<GPUGraphicsPipeline>(gpu);
        m_Compute = CreateScope<GPUComputePipeline>(gpu);
    }

    PipelineManager::~PipelineManager() = default;

    void PipelineManager::CreateGraphicsPipeline(const PipelineKey& key) {
        FALL_ASSERT_GPU_THREAD();
        FALL_ASSERT(key.type == PipelineType::Graphics);

        if (m_GraphicsKeys.insert(key).second) {
            m_Graphics->Create(key);
        }
    }

    void PipelineManager::CreateComputePipeline(const PipelineKey& key) {
        FALL_ASSERT_GPU_THREAD();
        FALL_ASSERT(key.type == PipelineType::Compute);

        if (m_ComputeKeys.insert(key).second) {
            m_Compute->Create(key);
        }
    }

    bool PipelineManager::HasGraphicsPipeline(const PipelineKey& key) const {
        return m_GraphicsKeys.contains(key);
    }

    bool PipelineManager::HasComputePipeline(const PipelineKey& key) const {
        return m_ComputeKeys.contains(key);
    }

}