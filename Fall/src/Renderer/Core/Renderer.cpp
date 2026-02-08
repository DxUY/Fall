#include "FallPCH.h"
#include "Renderer.h"

#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPUCommand.h"
#include "Renderer/GPU/GPURingBuffer.h"

#include "Renderer/Pass/Pass.h"

namespace Fall {

    Renderer::Renderer(GPUContext& gpu) : m_GPU(gpu) {
        m_Command = CreateScope<GPUCommand>(m_GPU);

        m_UniformRingBuffer = GPURingBuffer::CreateUniformBuffer(m_GPU, 1024 * 1024);
        m_ComputeRingBuffer = GPURingBuffer::CreateComputeBuffer(m_GPU, 2048 * 1024);
    }

    void Renderer::BeginFrame() {
        FALL_ASSERT_GPU_THREAD();

        m_Frame.Reset(++m_FrameIndex);
        m_Command->Begin();

        m_UniformRingBuffer->Reset();
        m_ComputeRingBuffer->Reset();

        BackbufferView bb = m_GPU.AcquireBackbuffer(m_Command->GetNative());

        if (!bb.IsValid()) return;

        m_Frame.SetBackbuffer(bb);
        m_Frame.SetCommand(m_Command.get());
    }

    void Renderer::EndFrame() {
        FALL_ASSERT_GPU_THREAD();

        if (m_Command->IsRecording()) {
            ExecuteFrame(m_Frame);
            m_Command->End();

            m_GPU.SyncCleanup(m_Command->GetLastFence());
        }
    }

    void Renderer::ExecuteFrame(FrameContext& frame) {
        GPUCommand& cmd = frame.GetCommand();
        for (const auto& pass : frame.GetPasses()) {
            if (pass) {
                pass->Execute(cmd, frame);
            }
        }
    }

}