#pragma once

#include "Renderer/Pass/RenderPass.h"
#include <glm/glm.hpp>

namespace Fall {

    class ClearPass : public RenderPass {
    public:
        ClearPass(const glm::vec4& color = { 0.1f, 0.1f, 0.1f, 1.0f })
            : m_ClearColor(color) {
        }

        virtual void Execute(GPUCommand& cmd, const FrameContext& frame) override;

        void SetColor(const glm::vec4& color) { m_ClearColor = color; }

    private:
        glm::vec4 m_ClearColor;
    };

}