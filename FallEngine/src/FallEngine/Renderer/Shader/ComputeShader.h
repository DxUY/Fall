#pragma once

#include "Renderer/Shader/ShaderReflection.h"
#include <vector>
#include <cstdint>

namespace FallEngine {

    class ComputeShader {
    public:
        ComputeShader(std::vector<uint8_t>&& bytecode, ShaderReflection&& reflection, uint32_t format)
            : m_Bytecode(std::move(bytecode)),
            m_Reflection(std::move(reflection)),
            m_Format(format) {
        }

        const std::vector<uint8_t>& GetBytecode() const { return m_Bytecode; }
        const ShaderReflection& GetReflection() const { return m_Reflection; }
        uint32_t GetFormat() const { return m_Format; }

    private:
        std::vector<uint8_t> m_Bytecode;
        ShaderReflection m_Reflection;
        uint32_t m_Format; 
    };

}