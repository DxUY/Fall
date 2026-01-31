#pragma once

namespace Fall {

    class GPUCommand;
    struct FrameContext;

    class ComputePass {
    public:
        virtual ~ComputePass() = default;

        virtual void Execute(
            GPUCommand& cmd,
            const FrameContext& frame
        ) = 0;
    };

}