#include "FallPCH.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlgpu3.h"

#include "Renderer/GPU/GPUContext.h"

#include "Renderer/Core/Renderer.h"
#include "Renderer/Core/FrameContext.h"

#include "Renderer/Pass/Implementations/ImGuiPass.h"

#include "Core/Application.h"

namespace Fall {
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

	void ImGuiLayer::OnAttach() {
		F_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		
		float fontSize = 18.0f;// *2.0f;
		io.Fonts->AddFontFromFileTTF("D:/dev/Sandbox/assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("D:/dev/Sandbox/assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);

		ImGui::StyleColorsDark();
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplSDLGPU3_InitInfo info{};
		info.Device = app.GetWindow().GetGPUContext().GetDevice();
		info.ColorTargetFormat = app.GetWindow().GetGPUContext().GetSwapchainFormat();
		info.PresentMode = app.GetWindow().IsVSync() ? SDL_GPU_PRESENTMODE_VSYNC : SDL_GPU_PRESENTMODE_IMMEDIATE;

		ImGui_ImplSDL3_InitForSDLGPU(window);
		ImGui_ImplSDLGPU3_Init(&info);
	}

	void ImGuiLayer::OnDetach() {
		F_PROFILE_FUNCTION();

		ImGui_ImplSDL3_Shutdown();
		ImGui_ImplSDLGPU3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& event) {
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			if (event.IsInCategory(EventCategoryMouse) && io.WantCaptureMouse) event.SetHandled(true);
			if (event.IsInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard) event.SetHandled(true);
		}
	}

	void ImGuiLayer::Begin() {
		F_PROFILE_FUNCTION();

		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		F_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();

		app.GetRenderer().GetFrame().AddPass(CreateScope<ImGuiPass>());
	}

	void ImGuiLayer::SetDarkThemeColors() {
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}

	uint32_t ImGuiLayer::GetActiveWidgetID() const {
		return GImGui->ActiveId;
	}
}