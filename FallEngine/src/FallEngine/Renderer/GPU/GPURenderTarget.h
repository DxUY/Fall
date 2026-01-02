#pragma once

#include "Renderer/Target/RenderTarget.h"
#include <SDL3/SDL_gpu.h>

namespace FallEngine {

	class GPUContext;
	class GPUCommand;

	class GPURenderTarget {
	public:
		GPURenderTarget(
			GPUContext& gpu,
			GPUCommand& cmd,
			const RenderTarget& desc
		);

		const SDL_GPUColorTargetInfo& GetColorInfo() const {
			return m_ColorInfo;
		}

	private:
		SDL_GPUColorTargetInfo m_ColorInfo{};
	};

}