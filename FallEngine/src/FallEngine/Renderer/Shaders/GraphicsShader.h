#pragma once

#include "Shader.h"

namespace FallEngine {

    class GraphicsShader : public Shader {
    public:
        GraphicsShader(SDL_GPUDevice* device);
        ~GraphicsShader();

        bool Load(const ShaderBlob& vertex, const ShaderBlob& fragment);

        SDL_GPUShader* GetVertexShader() const { return m_VertexShader; }
        SDL_GPUShader* GetFragmentShader() const { return m_FragmentShader; }

        void Release() override;
    private:
        SDL_GPUShader* m_VertexShader = nullptr;
        SDL_GPUShader* m_FragmentShader = nullptr;
    };

}