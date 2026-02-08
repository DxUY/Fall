#pragma once

namespace Fall {

	class GPUCommand;
	class FrameContext;

	class Pass {
	public:
		virtual ~Pass() = default;

		virtual void Execute(GPUCommand& cmd, const FrameContext& frame) = 0;
	};

}