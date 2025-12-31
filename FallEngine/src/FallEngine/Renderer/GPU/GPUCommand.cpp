#include "FallEnginePCH.h"
#include "GPUCommand.h"
#include "GPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

	struct GPUCommand::Impl
	{
		SDL_GPUCommandBuffer* CommandBuffer = nullptr;
		bool Recording = false;
	};

	GPUCommand::GPUCommand(GPUContext& gpu)
		: m_GPU(gpu), m_Impl(new Impl())
	{
	}

	GPUCommand::~GPUCommand()
	{
#ifdef FALL_DEBUG
		FALL_CORE_ASSERT(
			!m_Impl->Recording,
			"GPUCommandContext destroyed while recording"
		);
#endif
		delete m_Impl;
	}

	void GPUCommand::Begin()
	{
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(!m_Impl->Recording, "Command buffer already recording");

		m_Impl->CommandBuffer = SDL_AcquireGPUCommandBuffer(m_GPU.GetDevice());
		FALL_CORE_ASSERT(m_Impl->CommandBuffer, "Failed to acquire command buffer");

		m_Impl->Recording = true;
	}

	void GPUCommand::End()
	{
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(m_Impl->Recording, "End called without Begin");

		SDL_SubmitGPUCommandBuffer(m_Impl->CommandBuffer);
		m_Impl->CommandBuffer = nullptr;
		m_Impl->Recording = false;
	}

	bool GPUCommand::IsRecording() const
	{
		return m_Impl->Recording;
	}

}