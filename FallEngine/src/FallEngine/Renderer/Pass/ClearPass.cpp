#include "FallEnginePCH.h"
#include "ClearPass.h"

#include "Renderer/GPU/GPURenderTarget.h"
#include "Renderer/GPU/GPURenderPass.h"
#include "Renderer/GPU/GPUCommand.h"
#include "Renderer/GPU/GPUContext.h"

namespace FallEngine {

	ClearPass::ClearPass(GPUContext& gpu)
		: m_GPU(gpu)
	{
		m_Target = RenderTarget::Backbuffer({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void ClearPass::Execute(GPUCommand& cmd)
	{
		GPURenderTarget target(m_GPU, cmd, m_Target);
		GPURenderPass pass(cmd, target);
		// nothing else — clear happens here
	}

}
