#pragma once

#include "Renderer/GPU/GPUShader.h"
#include "Renderer/Shader/ShaderReflection.h"
#include "Core/Base.h"

namespace FallEngine {

    class GraphicsShader {
    public:
        GraphicsShader(Scope<GPUShader> gpuShader)
            : m_GPUShader(std::move(gpuShader)) {
        }

        const GPUShader& Get() const { return *m_GPUShader; }

    private:
        Scope<GPUShader> m_GPUShader;
    };

}