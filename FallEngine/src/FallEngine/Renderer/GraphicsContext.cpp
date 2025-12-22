#include "FallEnginePCH.h"
#include "GraphicsContext.h"
#include <thread>

namespace FallEngine {
	GraphicsContext::GraphicsContext(SDL_Window * windowHandle)
		: m_WindowHandle(windowHandle)
	{
		FALL_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

    GraphicsContext::~GraphicsContext() {
        SDL_WaitForGPUIdle(m_GPUDevice);
        SDL_ReleaseWindowFromGPUDevice(m_GPUDevice, m_WindowHandle);
        SDL_DestroyGPUDevice(m_GPUDevice);
    }

    void GraphicsContext::Init()
    {
        m_GPUDevice = SDL_CreateGPUDevice(
            SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
            true,
            nullptr
        );

        FALL_CORE_ASSERT(m_GPUDevice, "Failed to create GPU device");

        if (!SDL_ClaimWindowForGPUDevice(m_GPUDevice, m_WindowHandle)) {
			FALL_CORE_ERROR("Failed to claim window for GPU device");
            return;
        }
    }

}