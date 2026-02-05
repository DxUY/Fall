#include "FallPCH.h"
#include "GPUGraphicsPipeline.h"

#include "GPUContext.h"
#include "Renderer/Resource/Shader/GraphicsShader.h"

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPUGraphicsPipeline::GPUGraphicsPipeline(GPUContext& context)
        : m_Context(context) {
    }

    GPUGraphicsPipeline::~GPUGraphicsPipeline() {
        FALL_ASSERT_GPU_THREAD();

        for (auto& [key, pipeline] : m_Pipelines) {
            if (pipeline) {
                SDL_ReleaseGPUGraphicsPipeline(m_Context.GetDevice(), pipeline);
            }
        }

        m_Pipelines.clear();
    }

    bool GPUGraphicsPipeline::HasPipeline(const PipelineKey& key) const {
        return m_Pipelines.contains(key);
    }

    void GPUGraphicsPipeline::Create(const PipelineKey& key) {
        FALL_ASSERT_GPU_THREAD();

        if (HasPipeline(key)) return;

        if (!key.vertexShader || !key.fragmentShader || key.targetFormat == 0) return;

        SDL_GPUColorTargetDescription colorTarget{};
        colorTarget.format =
            static_cast<SDL_GPUTextureFormat>(key.targetFormat);

        if (key.useBlending) {
            colorTarget.blend_state.enable_blend = true;
            colorTarget.blend_state.src_color_blendfactor =
                SDL_GPU_BLENDFACTOR_SRC_ALPHA;
            colorTarget.blend_state.dst_color_blendfactor =
                SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
            colorTarget.blend_state.color_blend_op =
                SDL_GPU_BLENDOP_ADD;

            colorTarget.blend_state.src_alpha_blendfactor =
                SDL_GPU_BLENDFACTOR_ONE;
            colorTarget.blend_state.dst_alpha_blendfactor =
                SDL_GPU_BLENDFACTOR_ZERO;
            colorTarget.blend_state.alpha_blend_op =
                SDL_GPU_BLENDOP_ADD;
        }

        SDL_GPUGraphicsPipelineCreateInfo info{};
        info.target_info.num_color_targets = 1;
        info.target_info.color_target_descriptions = &colorTarget;
        info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

        info.vertex_shader = key.vertexShader->Get().GetNative();
        info.fragment_shader = key.fragmentShader->Get().GetNative();

        SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(m_Context.GetDevice(), &info);

        if (!pipeline) {
            FALL_CORE_ERROR("Failed to create graphics pipeline: {0}", SDL_GetError());
            return;
        }

        m_Pipelines.emplace(key, pipeline);
    }

    SDL_GPUGraphicsPipeline* GPUGraphicsPipeline::GetInternal(const PipelineKey& key) const {
        auto it = m_Pipelines.find(key);
        return (it != m_Pipelines.end()) ? it->second : nullptr;
    }

}