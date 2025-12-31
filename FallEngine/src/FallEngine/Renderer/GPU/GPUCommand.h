#pragma once

#include "FallEngine/Core/Base.h"

namespace FallEngine {

	class GPUContext;

	class GPUCommand {
	public:
		GPUCommand(GPUContext& gpu);
		~GPUCommand();

		FALL_NON_COPYABLE(GPUCommand);

		void Begin();   // Acquire command buffer
		void End();     // Submit command buffer

		bool IsRecording() const;

	private:
		GPUContext& m_GPU;
		struct Impl;
		Impl* m_Impl;   // PIMPL hides SDL types completely
	};

}
