#include "FallPCH.h"

#include "GPUShader.h"

#include "GPUContext.h"
#include "Renderer/Resource/Shader/ShaderReflection.h" 

#include <SDL3/SDL_gpu.h>

namespace Fall {

    GPUShader::GPUShader(GPUContext& gpu,
        const uint8_t* code,
        size_t codeSize,
        uint32_t format,
        uint32_t stage,
        const ShaderReflection& reflection)
        : m_GPU(gpu)
    {
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

        m_Shader = SDL_CreateGPUShader(gpu.GetDevice(), &info);

        FALL_CORE_ASSERT(m_Shader, "Failed to create GPU shader: {0}", SDL_GetError());
    }

    GPUShader::~GPUShader() {
        if (m_Shader) {
            SDL_ReleaseGPUShader(m_GPU.GetDevice(), m_Shader);
            m_Shader = nullptr;
        }
    }
}