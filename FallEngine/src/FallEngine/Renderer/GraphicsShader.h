#pragma once

#include "Shader.h"

namespace FallEngine {

    class GraphicsShader : public Shader {
    public:
        GraphicsShader(
            SDL_GPUDevice* device,
            const SDL_GPUVertexInputState& vertexInput
        );

        ~GraphicsShader();

        bool Load(
            const ShaderBlob& vertex,
            const ShaderBlob& fragment,
            SDL_GPUTextureFormat colorFormat
        );

        void Bind(SDL_GPURenderPass* renderPass);
        void Release() override;

    private:
        bool CreatePipeline();

    private:
        SDL_GPUShader* m_VertexShader = nullptr;
        SDL_GPUShader* m_FragmentShader = nullptr;
        SDL_GPUGraphicsPipeline* m_Pipeline = nullptr;

        SDL_GPUVertexInputState m_VertexInput;
        SDL_GPUTextureFormat m_ColorFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
    };

}
