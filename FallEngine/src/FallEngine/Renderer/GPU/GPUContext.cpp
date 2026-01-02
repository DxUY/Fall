#include "FallEnginePCH.h"
#include "GPUContext.h"

namespace FallEngine {

	GPUContext::GPUContext() {}

	GPUContext::~GPUContext()
	{
#ifdef FALL_DEBUG
		FALL_CORE_ASSERT(
			m_Device == nullptr,
			"GPUContext destroyed without explicit Shutdown()"
		);
#endif
	}

	bool GPUContext::Initialize(void* window)
	{
		SetGPUThreadID();
		FALL_ASSERT_GPU_THREAD();

		FALL_CORE_ASSERT(!m_Device, "GPUContext already initialized");
		FALL_CORE_ASSERT(window, "GPUContext::Initialize called with null window");

		m_Window = static_cast<SDL_Window*>(window);

		m_Device = SDL_CreateGPUDevice(
			SDL_GPU_SHADERFORMAT_SPIRV |
			SDL_GPU_SHADERFORMAT_DXIL |
			SDL_GPU_SHADERFORMAT_MSL,
			false,
			nullptr
		);

		if (!m_Device)
		{
			FALL_CORE_ERROR("Failed to create GPU device: {}", SDL_GetError());
			m_Window = nullptr;
			return false;
		}

		if (!SDL_ClaimWindowForGPUDevice(m_Device, m_Window))
		{
			FALL_CORE_ERROR("Failed to claim window for GPU device: {}", SDL_GetError());
			SDL_DestroyGPUDevice(m_Device);
			m_Device = nullptr;
			m_Window = nullptr;
			return false;
		}

		m_BackbufferFormat = SDL_GetGPUSwapchainTextureFormat(m_Device, m_Window);
		FALL_CORE_ASSERT(
			m_BackbufferFormat != SDL_GPU_TEXTUREFORMAT_INVALID,
			"Invalid backbuffer format"
		);

		FALL_CORE_INFO("GPUContext initialized");
		return true;
	}

	void GPUContext::Shutdown()
	{
		FALL_ASSERT_GPU_THREAD();

		if (m_Device)
		{
			SDL_WaitForGPUIdle(m_Device);
			SDL_ReleaseWindowFromGPUDevice(m_Device, m_Window);
			SDL_DestroyGPUDevice(m_Device);
			m_Device = nullptr;
		}

		m_Window = nullptr;
		m_BackbufferFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
	}

	SDL_GPUCommandBuffer* GPUContext::AcquireCommandBuffer()
	{
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(m_Device, "AcquireCommandBuffer called without device");

		SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(m_Device);
		FALL_CORE_ASSERT(cmd, "Failed to acquire GPU command buffer");
		return cmd;
	}

	void GPUContext::SubmitCommandBuffer(SDL_GPUCommandBuffer* cmd)
	{
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(cmd, "SubmitCommandBuffer called with null buffer");

		SDL_SubmitGPUCommandBuffer(cmd);
	}

	bool GPUContext::AcquireBackbuffer(
		SDL_GPUCommandBuffer* commandBuffer,
		SDL_GPUTexture** outTexture,
		uint32_t* outWidth,
		uint32_t* outHeight)
	{
		FALL_ASSERT_GPU_THREAD();

		return SDL_WaitAndAcquireGPUSwapchainTexture(
			commandBuffer,
			m_Window,
			outTexture,
			outWidth,
			outHeight
		);
	}

}