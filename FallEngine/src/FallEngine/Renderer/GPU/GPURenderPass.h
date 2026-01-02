#pragma once

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

	class GPUCommand;
	class GPURenderTarget;

	class GPURenderPass {
	public:
		GPURenderPass(
			GPUCommand& cmd,
			const GPURenderTarget& target
		);

		~GPURenderPass();

	private:
		SDL_GPURenderPass* m_Pass = nullptr;
	};

}