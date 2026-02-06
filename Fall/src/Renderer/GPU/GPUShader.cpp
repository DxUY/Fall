#include "FallPCH.h"
#include "GPUShader.h"

#include "GPUContext.h"

#include "Renderer/Resource/Shader/ShaderReflection.h" 

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPUShader::GPUShader(GPUContext& context,
        const uint8_t* code,
        size_t codeSize,
        uint32_t format,
        uint32_t stage,
        const ShaderReflection& reflection)
        : m_Context(context) {
        FALL_ASSERT_GPU_THREAD();

        SDL_GPUShaderCreateInfo info{};
        info.code = code;
        info.code_size = codeSize;
        info.entrypoint = reflection.entryPoint.c_str();
        info.format = static_cast<SDL_GPUShaderFormat>(format);
        info.stage = static_cast<SDL_GPUShaderStage>(stage);

        info.num_samplers = static_cast<uint32_t>(reflection.sampledTextures.size());
        info.num_uniform_buffers = static_cast<uint32_t>(reflection.uniformBuffers.size());

        info.num_storage_textures = static_cast<uint32_t>(
            reflection.readonlyStorageTextures.size() + reflection.readwriteStorageTextures.size());

        info.num_storage_buffers = static_cast<uint32_t>(
            reflection.readonlyStorageBuffers.size() + reflection.readwriteStorageBuffers.size());

        m_Shader = SDL_CreateGPUShader(context.GetDevice(), &info);

        FALL_CORE_ASSERT(m_Shader, "Failed to create GPU shader: {0}", SDL_GetError());
    }

    GPUShader::~GPUShader() {
        FALL_ASSERT_GPU_THREAD();
        ReleaseInternal();
    }

    void GPUShader::ReleaseInternal() {
        if (m_Shader) {
            SDL_GPUDevice* device = m_Context.GetDevice();
            SDL_GPUShader* shader = m_Shader;

            m_Context.EnqueueDeletion([device, shader]() {
                SDL_ReleaseGPUShader(device, shader);
                });

            m_Shader = nullptr;
        }
    }

}