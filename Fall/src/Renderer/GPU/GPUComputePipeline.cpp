#include "FallPCH.h"
#include "GPUComputePipeline.h"

#include "GPUContext.h"
#include "Renderer/Resource/Shader/ComputeShader.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPUComputePipeline::GPUComputePipeline(GPUContext& context)
        : m_Context(context) {}

    GPUComputePipeline::~GPUComputePipeline() {
        FALL_ASSERT_GPU_THREAD();

        for (auto& [key, pipeline] : m_Pipelines) {
            if (pipeline) {
                SDL_ReleaseGPUComputePipeline(m_Context.GetDevice(), pipeline);
            }
        }

        m_Pipelines.clear();
    }

    bool GPUComputePipeline::HasPipeline(const PipelineKey& key) const {
        return m_Pipelines.contains(key);
    }

    void GPUComputePipeline::Create(const PipelineKey& key) {
        FALL_ASSERT_GPU_THREAD();

        if (HasPipeline(key)) return;

        if (!key.computeShader) {
            FALL_CORE_ERROR("PipelineKey contains null computeShader pointer");
            return;
        }

        const ComputeShader& shader = *key.computeShader;

        SDL_GPUComputePipelineCreateInfo info{};
        info.code = shader.GetBytecode().data();
        info.code_size = shader.GetBytecode().size();
        info.format = static_cast<SDL_GPUShaderFormat>(shader.GetFormat());
        info.entrypoint = shader.GetReflection().entryPoint.c_str();

        info.threadcount_x = key.threadsX;
        info.threadcount_y = key.threadsY;
        info.threadcount_z = key.threadsZ;

        SDL_GPUComputePipeline* pipeline = SDL_CreateGPUComputePipeline(m_Context.GetDevice(), &info);

        if (!pipeline) {
            FALL_CORE_ERROR("Failed to create Compute Pipeline: {0}", SDL_GetError());
            return;
        }

        m_Pipelines.emplace(key, pipeline);
    }

    SDL_GPUComputePipeline* GPUComputePipeline::GetInternal(const PipelineKey& key) const {
        auto it = m_Pipelines.find(key);
        return (it != m_Pipelines.end()) ? it->second : nullptr;
    }

}