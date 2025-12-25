#include "FallEnginePCH.h"
#include "GraphicsShader.h"

namespace FallEngine {

    GraphicsShader::GraphicsShader(SDL_GPUDevice* device)
        : Shader(device) {
    }

    GraphicsShader::~GraphicsShader() {
        Release();
    }

    bool GraphicsShader::Load(const ShaderBlob& vertex, const ShaderBlob& fragment) {
        FALL_CORE_ASSERT(vertex.code && fragment.code, "ShaderBlob code is null");

        SDL_GPUShaderCreateInfo vert{};
        vert.code = vertex.code;
        vert.code_size = vertex.size;
        vert.entrypoint = vertex.entrypoint;
        vert.format = vertex.format;
        vert.stage = SDL_GPU_SHADERSTAGE_VERTEX;

        m_VertexShader = SDL_CreateGPUShader(m_Device, &vert);
        FALL_CORE_ASSERT(m_VertexShader, "Failed to create vertex shader");

        SDL_GPUShaderCreateInfo frag{};
        frag.code = fragment.code;
        frag.code_size = fragment.size;
        frag.entrypoint = fragment.entrypoint;
        frag.format = fragment.format;
        frag.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;

        m_FragmentShader = SDL_CreateGPUShader(m_Device, &frag);
        FALL_CORE_ASSERT(m_FragmentShader, "Failed to create fragment shader");

        return true;
    }

    void GraphicsShader::Release() {
        if (m_VertexShader) {
            SDL_ReleaseGPUShader(m_Device, m_VertexShader);
            m_VertexShader = nullptr;
        }
        if (m_FragmentShader) {
            SDL_ReleaseGPUShader(m_Device, m_FragmentShader);
            m_FragmentShader = nullptr;
        }
    }

}