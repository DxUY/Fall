#include "FallPCH.h"
#include "ImGuiPass.h"

#include "Renderer/Core/FrameContext.h"
#include "Renderer/GPU/GPUCommand.h"
#include "Renderer/GPU/GPURenderTarget.h"
#include "Renderer/GPU/GPURenderPass.h"

#include "Renderer/Resource/Target/RenderTarget.h" 

#include <imgui.h>
#include <imgui_impl_sdlgpu3.h> 

namespace Fall {
    void ImGuiPass::Execute(GPUCommand& cmd, const FrameContext& frame) {
        F_PROFILE_FUNCTION();

        ImDrawData* drawData = ImGui::GetDrawData();
        if (!drawData || drawData->CmdListsCount == 0) return;

        const auto& backbuffer = frame.GetBackbuffer();
        if (!backbuffer.IsValid()) return;

        GPURenderTarget target(backbuffer, RenderTarget::Backbuffer());
        {
            GPURenderPass pass(cmd, target);

            ImGui_ImplSDLGPU3_RenderDrawData(
                drawData,
                cmd.GetNative(),
                pass.GetNative(),
                nullptr
            );
        }
    }
}