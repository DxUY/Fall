#include "FallPCH.h"
#include "Renderer.h"

#include "Renderer/GPU/GPUContext.h"
#include "Renderer/GPU/GPUCommand.h"

#include "Renderer/Pass/RenderPass.h"

namespace Fall {

    Renderer::Renderer(GPUContext& gpu) : m_GPU(gpu) {
		m_Command = CreateScope<GPUCommand>(m_GPU);
    }

    Renderer::~Renderer() {}

    void Renderer::BeginFrame() {
        FALL_ASSERT_GPU_THREAD();
        m_Frame.Reset(++m_FrameIndex);
        m_Command->Begin();

        BackbufferView bb = m_GPU.AcquireBackbuffer(m_Command->GetNative());

        m_Frame.SetBackbuffer(bb);
        m_Frame.SetCommand(m_Command.get());
    }

    void Renderer::EndFrame() {
        FALL_ASSERT_GPU_THREAD();
        ExecuteFrame(m_Frame);
        m_Command->End();
    }

    void Renderer::ExecuteFrame(FrameContext& frame) {
        GPUCommand& cmd = frame.GetCommand();
        for (RenderPass* pass : frame.GetPasses()) {
            pass->Execute(cmd, frame);
        }
    }

}