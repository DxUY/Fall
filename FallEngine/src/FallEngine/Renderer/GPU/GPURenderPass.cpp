#include "FallEnginePCH.h"

#include "GPURenderPass.h"
#include "GPUCommand.h"
#include "GPURenderTarget.h"

namespace FallEngine {

	GPURenderPass::GPURenderPass(
		GPUCommand& cmd,
		const GPURenderTarget& target
	) {
		m_Pass = SDL_BeginGPURenderPass(
			cmd.GetNativeCommandBuffer(),
			&target.GetColorInfo(),
			1,
			nullptr
		);
	}

	GPURenderPass::~GPURenderPass()
	{
		SDL_EndGPURenderPass(m_Pass);
	}

}