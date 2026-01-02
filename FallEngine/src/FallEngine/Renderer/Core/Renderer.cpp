#include "FallEnginePCH.h"
#include "Renderer.h"

#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPUCommand.h"

#include "Renderer/Pass/ClearPass.h"

namespace FallEngine {

	Renderer::Renderer(GPUContext& gpu)
		: m_GPU(gpu)
	{
		m_Command = new GPUCommand(m_GPU);
	}

	Renderer::~Renderer()
	{
		delete m_Command;
	}

	void Renderer::BeginFrame()
	{
		FALL_ASSERT_GPU_THREAD();

		m_Frame.Reset(++m_FrameIndex);
		m_Command->Begin();

		ClearPass clear(m_GPU);
		clear.Execute(*m_Command);
	}

	void Renderer::EndFrame()
	{
		FALL_ASSERT_GPU_THREAD();
		m_Command->End();
	}

}