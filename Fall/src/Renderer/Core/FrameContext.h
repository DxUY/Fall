#pragma once

#include "Core/Base.h"
#include "Renderer/GPU/GPUContext.h" 
#include <cstdint>
#include <vector>

namespace Fall {

    class GPUCommand;
    class Pass;    
    class Camera;

    class FrameContext {
    public:
        FrameContext() = default;
        ~FrameContext() = default;

        FALL_NON_COPYABLE(FrameContext)

        void Reset(uint64_t newFrameIndex);

        uint64_t GetFrameIndex() const { return m_FrameIndex; }

        GPUCommand& GetCommand() const;
        const BackbufferView& GetBackbuffer() const;

        void SetCamera(Camera* camera) { m_Camera = camera; }
        Camera* GetCamera() const { return m_Camera; }

        void AddPass(Scope<Pass> pass);
        const std::vector<Scope<Pass>>& GetPasses() const;

    private:
        friend class Renderer;

        void SetCommand(GPUCommand* command);
        void SetBackbuffer(const BackbufferView& view);

    private:
        uint64_t m_FrameIndex = 0;
        GPUCommand* m_Command = nullptr;
        BackbufferView m_Backbuffer;
        std::vector<Scope<Pass>> m_Passes;

        Camera* m_Camera = nullptr;
    };
}