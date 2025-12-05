#include "FallEnginePCH.h"
#include "GraphicsContext.h"
#include "FallEngine/Renderer/Resources/Shader.h"

namespace FallEngine {
	GraphicsContext::GraphicsContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		FALL_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void GraphicsContext::Init()
	{
		m_GPUDevice = SDL_CreateGPUDevice(
			SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
			true,
			NULL);

		if (!m_GPUDevice)
		{
			FALL_CORE_ERROR("Failed to create SDL GPU Device: {0}", SDL_GetError());
			FALL_CORE_ASSERT(false, "Failed to create GPU Device!");
			return;
		}

		if (!SDL_ClaimWindowForGPUDevice(m_GPUDevice, m_WindowHandle))
		{
			FALL_CORE_ERROR("Failed to claim window for SDL GPU Device: {0}", SDL_GetError());
			FALL_CORE_ASSERT(false, "Failed to claim window!");
			return;
		}
	}

    void GraphicsContext::SwapBuffers() {
    
	}
}