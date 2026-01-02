#pragma once

#include "FallEngine/Core/Base.h"
#include "FrameContext.h"

namespace FallEngine {

	class GPUContext;
	class GPUCommand;

	class Renderer {
	public:
		Renderer(GPUContext& gpu);
		~Renderer();

		FALL_NON_COPYABLE(Renderer);

		void BeginFrame();
		void EndFrame();

	private:
		GPUContext& m_GPU;
		GPUCommand* m_Command = nullptr;

		FrameContext m_Frame;
		uint64_t m_FrameIndex = 0;
	};

}