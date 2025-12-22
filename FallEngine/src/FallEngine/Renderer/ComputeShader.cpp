#include "ComputeShader.h"

namespace FallEngine {

    ComputeShader::ComputeShader(SDL_GPUDevice* device, uint32_t threadX, uint32_t threadY, uint32_t threadZ)
        : Shader(device), m_ThreadX(threadX), m_ThreadY(threadY), m_ThreadZ(threadZ) {}

    ComputeShader::~ComputeShader() {
        Release();
    }

    bool ComputeShader::Load(const ShaderBlob& blob) {
        FALL_CORE_ASSERT(!m_Pipeline, "ComputeShader already loaded");
        FALL_CORE_ASSERT(blob.code, "Compute shader blob is null");

        SDL_GPUComputePipelineCreateInfo info{};
        info.code = blob.code;
        info.code_size = blob.size;
        info.format = blob.format;
        info.entrypoint = blob.entrypoint;

        info.threadcount_x = m_ThreadX;
        info.threadcount_y = m_ThreadY;
        info.threadcount_z = m_ThreadZ;

        m_Pipeline = SDL_CreateGPUComputePipeline(m_Device, &info);
        FALL_CORE_ASSERT(m_Pipeline, "Failed to create compute pipeline");

        return true;
    }

    void ComputeShader::Bind(SDL_GPUComputePass* pass) {
        FALL_CORE_ASSERT(m_Pipeline, "Compute pipeline not loaded");
        FALL_CORE_ASSERT(pass, "Compute pass is null");

        m_ActivePass = pass;
        SDL_BindGPUComputePipeline(pass, m_Pipeline);
    }

    void ComputeShader::Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) {
        FALL_CORE_ASSERT(m_ActivePass, "Dispatch called without Bind()");
        SDL_DispatchGPUCompute(m_ActivePass, groupsX, groupsY, groupsZ);
    }

    void ComputeShader::Release() {
        if (m_Pipeline) {
            SDL_ReleaseGPUComputePipeline(m_Device, m_Pipeline);
            m_Pipeline = nullptr;
        }

        m_ActivePass = nullptr;
    }

}