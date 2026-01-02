#pragma once
#include "Renderer/Target/RenderTarget.h"

namespace FallEngine {

	class GPUCommand;

	class RenderPass {
	public:
		virtual ~RenderPass() = default;
		virtual void Execute(GPUCommand& cmd) = 0;
	};

}