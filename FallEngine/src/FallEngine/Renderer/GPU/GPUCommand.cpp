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
		: m_Impl(CreateScope<Impl>()), m_GPU(gpu) {
	}

	GPUCommand::~GPUCommand() {
#ifdef FALL_DEBUG
		FALL_CORE_ASSERT(!m_Impl->Recording, "GPUCommand destroyed while still recording");
#endif
	}

	void GPUCommand::Begin() {
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(!m_Impl->Recording, "Already recording");

		m_Impl->Cmd = m_GPU.AcquireCommandBuffer();
		
		if (!m_Impl->Cmd) {
			FALL_CORE_ERROR("Failed to acquire GPU command buffer");
			m_Impl->Recording = false;
			return;
		}

		m_Impl->Recording = true;
	}

	void GPUCommand::End() {
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(m_Impl->Recording, "Ending a command buffer that wasn't started");

		if (!m_Impl->Cmd) {
			FALL_CORE_ERROR("Attempting to submit null command buffer");
		} else {
			m_GPU.SubmitCommandBuffer(m_Impl->Cmd);
		}
		
		m_Impl->Cmd = nullptr;
		m_Impl->Recording = false;
	}

	bool GPUCommand::IsRecording() const {
		return m_Impl->Recording;
	}

	SDL_GPUCommandBuffer* GPUCommand::GetNative() const {
		FALL_CORE_ASSERT(m_Impl->Cmd, "Attempting to access null command buffer (Did you call Begin()?)");
		return m_Impl->Cmd;
	}

}