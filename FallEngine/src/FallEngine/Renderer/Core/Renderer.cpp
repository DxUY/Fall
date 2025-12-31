#include "FallEnginePCH.h"
#include "Renderer.h"

#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPUCommand.h"

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

		m_Command->Begin();

		// Backbuffer acquisition will be added here later
	}

	void Renderer::EndFrame()
	{
		FALL_ASSERT_GPU_THREAD();

		m_Command->End();
	}

}