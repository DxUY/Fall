#pragma once

#include "Core/Base.h"
#include "FrameContext.h"

namespace Fall {

    class GPUContext;
    class GPUCommand;
    class GPURingBuffer;

    class Renderer {
    public:
        Renderer(GPUContext& gpu);
        virtual ~Renderer() = default;

        FALL_NON_COPYABLE(Renderer);

        FrameContext& GetFrame() { return m_Frame; }

        GPURingBuffer& GetUniformRingBuffer() { return *m_UniformRingBuffer; }
        GPURingBuffer& GetComputeRingBuffer() { return *m_ComputeRingBuffer; }

        void BeginFrame();
        void EndFrame();

    private:
        void ExecuteFrame(FrameContext& frame);

    private:
        GPUContext& m_GPU;

        Scope<GPUCommand> m_Command;

        Scope<GPURingBuffer> m_UniformRingBuffer;
        Scope<GPURingBuffer> m_ComputeRingBuffer;

        FrameContext m_Frame;
        uint64_t m_FrameIndex = 0;
    };

}