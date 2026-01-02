#pragma once

#include "Renderer/Pass/RenderPass.h"

namespace FallEngine {

	class GPUContext;

	class ClearPass : public RenderPass {
	public:
		explicit ClearPass(GPUContext& gpu);

		void Execute(GPUCommand& cmd) override;

	private:
		GPUContext& m_GPU;
		RenderTarget m_Target;
	};

}
