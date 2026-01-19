#include "FallEnginePCH.h"
#include "FrameContext.h"

#include "Renderer/Pass/RenderPass.h"
#include "Renderer/GPU/GPUCommand.h"

namespace FallEngine {

	void FrameContext::Reset(uint64_t newFrameIndex)
	{
		m_FrameIndex = newFrameIndex;
		m_Command = nullptr;
		m_Backbuffer = {};
		m_Passes.clear();
	}

	void FrameContext::SetCommand(GPUCommand* command)
	{
		m_Command = command;
	}

	void FrameContext::SetBackbuffer(const BackbufferView& view)
	{
		m_Backbuffer = view;
	}

	GPUCommand& FrameContext::GetCommand() const
	{
		FALL_CORE_ASSERT(m_Command, "FrameContext has no command buffer");
		return *m_Command;
	}

	const BackbufferView& FrameContext::GetBackbuffer() const
	{
		return m_Backbuffer;
	}

	void FrameContext::AddPass(RenderPass* pass)
	{
		FALL_CORE_ASSERT(pass, "Null RenderPass");
		m_Passes.push_back(pass);
	}

	const std::vector<RenderPass*>& FrameContext::GetPasses() const
	{
		return m_Passes;
	}

}