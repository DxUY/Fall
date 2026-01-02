#pragma once

#include <SDL3/SDL_gpu.h>
#include "FallEngine/Core/Base.h"

struct SDL_Window;

namespace FallEngine {

	class GPUCommand;

	class GPUContext {
	public:
		GPUContext();
		~GPUContext();

		FALL_NON_COPYABLE(GPUContext);

		bool Initialize(void* window);
		void Shutdown();

		SDL_GPUCommandBuffer* AcquireCommandBuffer();
		void SubmitCommandBuffer(SDL_GPUCommandBuffer* cmd);

		bool AcquireBackbuffer(
			SDL_GPUCommandBuffer* commandBuffer,
			SDL_GPUTexture** outTexture,
			uint32_t* outWidth,
			uint32_t* outHeight
		);

		SDL_GPUTextureFormat GetBackbufferFormat() const { return m_BackbufferFormat; }

	private:
		SDL_Window* m_Window = nullptr;
		SDL_GPUDevice* m_Device = nullptr;
		SDL_GPUTextureFormat m_BackbufferFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
	};

}