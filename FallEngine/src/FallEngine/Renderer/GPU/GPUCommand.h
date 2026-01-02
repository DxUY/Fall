#pragma once

#include "FallEngine/Core/Base.h"

struct SDL_GPUCommandBuffer;

namespace FallEngine {

	class GPUContext;

	class GPUCommand {
	public:
		GPUCommand(GPUContext& gpu);
		~GPUCommand();

		FALL_NON_COPYABLE(GPUCommand);

		void Begin();
		void End();

		bool IsRecording() const;

	private:
		SDL_GPUCommandBuffer* GetNativeCommandBuffer() const;
	
	private:
		friend class GPURenderPass;
		friend class GPURenderTarget;

		struct Impl;
		Impl* m_Impl;

		GPUContext& m_GPU;
	};

}