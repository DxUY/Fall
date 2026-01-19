#include "FallEnginePCH.h"
#include "GPUPipeline.h"
#include "GPUContext.h"

#include "Renderer/Shader/GraphicsShader.h"
#include "Renderer/Shader/ComputeShader.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    GPUPipeline::GPUPipeline(GPUContext& gpu)
        : m_GPU(gpu) {}

    void* GPUPipeline::CreateAndRegister(PipelineHandle handle, const PipelineKey& key) {
        FALL_ASSERT_GPU_THREAD();

        void* nativePtr = nullptr;
        if (key.type == PipelineType::Compute) {
            nativePtr = CreateCompute(key);
            if (nativePtr) m_GPU.GetComputePipelineRegistry().Register(handle, (SDL_GPUComputePipeline*)nativePtr);
        }
        else {
            nativePtr = CreateGraphics(key);
            if (nativePtr) m_GPU.GetGraphicsPipelineRegistry().Register(handle, (SDL_GPUGraphicsPipeline*)nativePtr);
        }
        return nativePtr;
    }

    SDL_GPUGraphicsPipeline* GPUPipeline::CreateGraphics(const PipelineKey& key) {
        if (!key.vertexShader && !key.fragmentShader && key.targetFormat == 0) {
            return nullptr;
        }

        if (!key.vertexShader || !key.fragmentShader) {
            FALL_CORE_ERROR("Pipeline creation failed: Missing Vertex or Fragment shader!");
            return nullptr;
        }

        SDL_GPUColorTargetDescription colorTarget{};

        FALL_CORE_INFO("Creating graphics pipeline with target format: {0}", key.targetFormat);

        colorTarget.format = static_cast<SDL_GPUTextureFormat>(key.targetFormat);

        if (key.useBlending) {
            colorTarget.blend_state.enable_blend = true;
            colorTarget.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
            colorTarget.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
            colorTarget.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
            colorTarget.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
            colorTarget.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;
            colorTarget.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
        }

        SDL_GPUGraphicsPipelineCreateInfo info{};
        info.target_info.num_color_targets = 1;
        info.target_info.color_target_descriptions = &colorTarget;
        info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

        info.vertex_shader = key.vertexShader->Get().GetNative();
        info.fragment_shader = key.fragmentShader->Get().GetNative();

        SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(m_GPU.GetDevice(), &info);

        if (!pipeline) {
            FALL_CORE_ERROR("SDL_CreateGPUGraphicsPipeline failed: {0}", SDL_GetError());
        }

        return pipeline;
    }

    SDL_GPUComputePipeline* GPUPipeline::CreateCompute(const PipelineKey& key) {
        if (!key.computeShader && (key.threadsX == 0 || key.threadsY == 0 || key.threadsZ)) {
            return nullptr;
        }

        if (!key.computeShader) {
            FALL_CORE_ERROR("Compute Pipeline creation failed: Missing Compute shader!");
            return nullptr;
        }

        const auto& bytecode = key.computeShader->GetBytecode();
        const auto& reflection = key.computeShader->GetReflection();

        SDL_GPUComputePipelineCreateInfo info{};
        info.code = bytecode.data();
        info.code_size = bytecode.size();
        info.format = static_cast<SDL_GPUShaderFormat>(key.computeShader->GetFormat());
        info.entrypoint = reflection.entryPoint.c_str();

        info.num_samplers = static_cast<uint32_t>(reflection.sampledTextures.size());
        info.num_uniform_buffers = static_cast<uint32_t>(reflection.uniformBuffers.size());

        info.num_readonly_storage_textures = static_cast<uint32_t>(reflection.readonlyStorageTextures.size());
        info.num_readwrite_storage_textures = static_cast<uint32_t>(reflection.readwriteStorageTextures.size());

        info.num_readonly_storage_buffers = static_cast<uint32_t>(reflection.readonlyStorageBuffers.size());
        info.num_readwrite_storage_buffers = static_cast<uint32_t>(reflection.readwriteStorageBuffers.size());

        info.threadcount_x = key.threadsX;
        info.threadcount_y = key.threadsY;
        info.threadcount_z = key.threadsZ;

        return SDL_CreateGPUComputePipeline(m_GPU.GetDevice(), &info);
    }
}