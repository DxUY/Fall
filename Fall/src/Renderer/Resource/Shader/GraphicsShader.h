#pragma once

#include "Renderer/GPU/GPUShader.h"
#include "Renderer/Resource/Shader/ShaderReflection.h"
#include "Core/Base.h"

namespace Fall {

    class GraphicsShader {
    public:
        GraphicsShader(Scope<GPUShader> gpuShader, ShaderReflection&& reflection)
            : m_GPUShader(std::move(gpuShader)), m_Reflection(std::move(reflection)) {
        }

        inline const GPUShader& Get() const { return *m_GPUShader; }
        inline const ShaderReflection& GetReflection() const { return m_Reflection; }

    private:
        Scope<GPUShader> m_GPUShader;
        ShaderReflection m_Reflection;
    };

}