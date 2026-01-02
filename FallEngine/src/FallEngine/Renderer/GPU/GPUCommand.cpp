#include "FallEnginePCH.h"
#include "GPUCommand.h"
#include "GPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

	struct GPUCommand::Impl {
		SDL_GPUCommandBuffer* Cmd = nullptr;
		bool Recording = false;
	};

	GPUCommand::GPUCommand(GPUContext& gpu)
		: m_Impl(new Impl()), m_GPU(gpu) {}

	GPUCommand::~GPUCommand() {
#ifdef FALL_DEBUG
		FALL_CORE_ASSERT(
			!m_Impl->Recording,
			"GPUCommand destroyed while recording"
		);
#endif
		delete m_Impl;
	}

	void GPUCommand::Begin() {
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(!m_Impl->Recording, "Command already recording");

		m_Impl->Cmd = m_GPU.AcquireCommandBuffer();
		m_Impl->Recording = true;
	}

	void GPUCommand::End() {
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(m_Impl->Recording, "End called without Begin");

		m_GPU.SubmitCommandBuffer(m_Impl->Cmd);
		m_Impl->Cmd = nullptr;
		m_Impl->Recording = false;
	}

	bool GPUCommand::IsRecording() const {
		return m_Impl->Recording;
	}

	SDL_GPUCommandBuffer* GPUCommand::GetNativeCommandBuffer() const { 
		return m_Impl->Cmd; 
	}

}