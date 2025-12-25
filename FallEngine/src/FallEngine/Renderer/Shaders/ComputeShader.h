#pragma once

#include "Shader.h"

namespace FallEngine {

    class ComputeShader : public Shader {
    public:
        ComputeShader(SDL_GPUDevice* device, uint32_t threadX, uint32_t threadY, uint32_t threadZ);
		~ComputeShader();

        bool Load(ShaderBlob&& blob);

        const ShaderBlob& GetBlob() const { return m_Blob; }

        uint32_t ThreadX() const { return m_ThreadX; }
        uint32_t ThreadY() const { return m_ThreadY; }
        uint32_t ThreadZ() const { return m_ThreadZ; }

        void Release() override;

    private:
        ShaderBlob m_Blob{};

        uint32_t m_ThreadX = 1;
        uint32_t m_ThreadY = 1;
        uint32_t m_ThreadZ = 1;
    };

}
