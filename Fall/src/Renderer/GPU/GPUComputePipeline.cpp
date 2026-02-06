#include "FallPCH.h"
#include "GPUComputePipeline.h"

#include "GPUContext.h"
#include "Renderer/Resource/Shader/ComputeShader.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPUComputePipeline::GPUComputePipeline(GPUContext& context)
        : m_Context(context) {
    }

    GPUComputePipeline::~GPUComputePipeline() {
        FALL_ASSERT_GPU_THREAD();
        ReleaseInternal();
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

        info.num_readonly_storage_textures = static_cast<uint32_t>(shader.GetReflection().readonlyStorageTextures.size());
        info.num_readonly_storage_buffers = static_cast<uint32_t>(shader.GetReflection().readonlyStorageBuffers.size());
        info.num_readwrite_storage_textures = static_cast<uint32_t>(shader.GetReflection().readwriteStorageTextures.size());
        info.num_readwrite_storage_buffers = static_cast<uint32_t>(shader.GetReflection().readwriteStorageBuffers.size());
        info.num_uniform_buffers = static_cast<uint32_t>(shader.GetReflection().uniformBuffers.size());

        SDL_GPUComputePipeline* pipeline = SDL_CreateGPUComputePipeline(m_Context.GetDevice(), &info);

        if (!pipeline) {
            FALL_CORE_ERROR("Failed to create Compute Pipeline: {0}", SDL_GetError());
            return;
        }

        m_Pipelines.emplace(key, pipeline);
    }

    void GPUComputePipeline::ReleaseInternal() {
        if (m_Pipelines.empty()) return;

        auto pipelinesToRelease = std::move(m_Pipelines);
        SDL_GPUDevice* device = m_Context.GetDevice();

        m_Context.EnqueueDeletion([device, pipelines = std::move(pipelinesToRelease)]() {
            for (auto& [key, pipeline] : pipelines) {
                if (pipeline) {
                    SDL_ReleaseGPUComputePipeline(device, pipeline);
                }
            }
            });

        m_Pipelines.clear();
    }

    SDL_GPUComputePipeline* GPUComputePipeline::GetInternal(const PipelineKey& key) const {
        auto it = m_Pipelines.find(key);
        return (it != m_Pipelines.end()) ? it->second : nullptr;
    }

}