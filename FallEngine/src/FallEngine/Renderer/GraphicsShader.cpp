#include "GraphicsShader.h"

namespace FallEngine {

    GraphicsShader::GraphicsShader(SDL_GPUDevice* device, const SDL_GPUVertexInputState& vertexInput) 
        : Shader(device), m_VertexInput(vertexInput) {}

    GraphicsShader::~GraphicsShader() {
        Release();
    }

    bool GraphicsShader::Load(const ShaderBlob& vertex, const ShaderBlob& fragment, SDL_GPUTextureFormat colorFormat) {
        FALL_CORE_ASSERT(!m_Pipeline, "GraphicsShader already loaded");
        FALL_CORE_ASSERT(vertex.code && fragment.code, "ShaderBlob code is null");

        m_ColorFormat = colorFormat;

        SDL_GPUShaderCreateInfo vertInfo{};
        vertInfo.code = vertex.code;
        vertInfo.code_size = vertex.size;
        vertInfo.entrypoint = vertex.entrypoint;
        vertInfo.format = vertex.format;
        vertInfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;

        m_VertexShader = SDL_CreateGPUShader(m_Device, &vertInfo);
        FALL_CORE_ASSERT(m_VertexShader, "Failed to create vertex shader");

        SDL_GPUShaderCreateInfo fragInfo{};
        fragInfo.code = fragment.code;
        fragInfo.code_size = fragment.size;
        fragInfo.entrypoint = fragment.entrypoint;
        fragInfo.format = fragment.format;
        fragInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;

        m_FragmentShader = SDL_CreateGPUShader(m_Device, &fragInfo);
        FALL_CORE_ASSERT(m_FragmentShader, "Failed to create fragment shader");

        return CreatePipeline();
    }

    bool GraphicsShader::CreatePipeline() {
        FALL_CORE_ASSERT(m_VertexShader && m_FragmentShader, "Shaders not created");

        SDL_GPUColorTargetDescription colorTarget{};
        colorTarget.format = m_ColorFormat;

        SDL_GPUGraphicsPipelineCreateInfo info{};
        info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
        info.vertex_input_state = m_VertexInput;
        info.vertex_shader = m_VertexShader;
        info.fragment_shader = m_FragmentShader;

        info.target_info.num_color_targets = 1;
        info.target_info.color_target_descriptions = &colorTarget;

        m_Pipeline = SDL_CreateGPUGraphicsPipeline(m_Device, &info);
        FALL_CORE_ASSERT(m_Pipeline, "Failed to create graphics pipeline");

        return true;
    }

    void GraphicsShader::Bind(SDL_GPURenderPass* renderPass) {
        FALL_CORE_ASSERT(m_Pipeline, "Pipeline not loaded");
        FALL_CORE_ASSERT(renderPass, "Render pass is null");

        SDL_BindGPUGraphicsPipeline(renderPass, m_Pipeline);
    }

    void GraphicsShader::Release() {
        if (m_Pipeline) {
            SDL_ReleaseGPUGraphicsPipeline(m_Device, m_Pipeline);
            m_Pipeline = nullptr;
        }

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