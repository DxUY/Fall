#include "FallPCH.h"
#include "FrameContext.h"

#include "Renderer/Pass/Pass.h"
#include "Camera.h"
#include "Renderer/GPU/GPUCommand.h"

namespace Fall {

	void FrameContext::Reset(uint64_t newFrameIndex)
	{
		m_FrameIndex = newFrameIndex;
		m_Command = nullptr;
		m_Backbuffer = {};
		m_Passes.clear();

		m_Camera = nullptr;
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

	void FrameContext::AddPass(Scope<Pass> pass) {
		FALL_CORE_ASSERT(pass, "Null Pass");
		m_Passes.push_back(std::move(pass));
	}

	const std::vector<Scope<Pass>>& FrameContext::GetPasses() const
	{
		return m_Passes;
	}

}