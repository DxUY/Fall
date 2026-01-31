#pragma once

namespace Fall {

	class GPUCommand;
	class FrameContext;

	class RenderPass {
	public:
		virtual ~RenderPass() = default;

		virtual void Execute(
			GPUCommand& cmd,
			const FrameContext& frame
		) = 0;
	};

}