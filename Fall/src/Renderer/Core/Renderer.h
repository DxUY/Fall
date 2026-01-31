#pragma once

#include "Core/Base.h"
#include "FrameContext.h"

namespace Fall {

	class GPUContext;
	class GPUCommand;

	class Renderer {
	public:
		Renderer(GPUContext& gpu);
		~Renderer();

		FALL_NON_COPYABLE(Renderer);

		FrameContext& GetFrame() { return m_Frame; }

		void BeginFrame();
		void EndFrame();

	private:
		void ExecuteFrame(FrameContext& frame);

	private:
		GPUContext& m_GPU;
		Scope<GPUCommand> m_Command;

		FrameContext m_Frame;
		uint64_t m_FrameIndex = 0;
	};

}