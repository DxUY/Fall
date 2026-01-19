#pragma once

#include "FallEngine/Core/Base.h"

struct SDL_GPUCommandBuffer;

namespace FallEngine {

	class GPUContext;

	class GPUCommand {
	public:
		GPUCommand(GPUContext& gpu);
		~GPUCommand();

		FALL_NON_COPYABLE(GPUCommand)

		void Begin();
		void End();

		bool IsRecording() const;

		SDL_GPUCommandBuffer* GetNative() const;
		GPUContext& GetGPUContext() const { return m_GPU; }

	private:
		struct Impl;
		Scope<Impl> m_Impl;
		GPUContext& m_GPU;
	};

}