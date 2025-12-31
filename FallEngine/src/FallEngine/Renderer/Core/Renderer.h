#pragma once

#include "FallEngine/Core/Base.h"

namespace FallEngine {

	class GPUContext;
	class GPUCommand;

	class Renderer {
	public:
		explicit Renderer(GPUContext& gpu);
		~Renderer();

		FALL_NON_COPYABLE(Renderer);

		void BeginFrame();
		void EndFrame();

	private:
		GPUContext& m_GPU;
		GPUCommand* m_Command = nullptr;
	};

}