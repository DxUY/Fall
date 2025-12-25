#include "FallEnginePCH.h"
#include "ComputeShader.h"

namespace FallEngine {

    ComputeShader::ComputeShader(SDL_GPUDevice* device, uint32_t threadX, uint32_t threadY, uint32_t threadZ)
        : Shader(device), m_ThreadX(threadX), m_ThreadY(threadY), m_ThreadZ(threadZ) {
    }

    ComputeShader::~ComputeShader() {
        Release();
	}

    bool ComputeShader::Load(ShaderBlob&& blob) {
        FALL_CORE_ASSERT(blob.code, "Compute shader blob is null");
        m_Blob = std::move(blob);
        return true;
    }

    void ComputeShader::Release() {
        m_Blob = {};
    }

}
