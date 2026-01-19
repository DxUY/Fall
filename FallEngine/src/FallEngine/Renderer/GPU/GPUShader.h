#pragma once

#include <cstdint>
#include "FallEngine/Core/Base.h" // Assuming macro is here

struct SDL_GPUShader;

namespace FallEngine {

    class GPUContext;
    struct ShaderReflection;

    class GPUShader {
    public:
        GPUShader(GPUContext& gpu,
            const uint8_t* code,
            size_t codeSize,
            uint32_t format,
            uint32_t stage,
            const ShaderReflection& reflection);

        ~GPUShader();

        FALL_NON_COPYABLE(GPUShader)

            SDL_GPUShader* GetNative() const { return m_Shader; }

    private:
        GPUContext& m_GPU;
        SDL_GPUShader* m_Shader = nullptr;
    };
}