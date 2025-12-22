#pragma once

#include "Shader.h"

namespace FallEngine {

    class ComputeShader : public Shader {
    public:
        ComputeShader(
            SDL_GPUDevice* device,
            uint32_t threadX,
            uint32_t threadY,
            uint32_t threadZ
        );

        ~ComputeShader();

        bool Load(const ShaderBlob& blob);
        void Bind(SDL_GPUComputePass* pass);
        void Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ);
        void Release() override;

    private:
        SDL_GPUComputePipeline* m_Pipeline = nullptr;
        SDL_GPUComputePass* m_ActivePass = nullptr;

        uint32_t m_ThreadX = 1;
        uint32_t m_ThreadY = 1;
        uint32_t m_ThreadZ = 1;
    };

}