#include "ImGuiLayer.h"
#include "FallEnginePCH.h"

#include "FallEngine/Core/Application.h"


namespace FallEngine {

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {}

    ImGuiLayer::~ImGuiLayer()
    {}

    void ImGuiLayer::OnAttach()
    {
        //IMGUI_CHECKVERSION();
        //ImGui::CreateContext();
        //ImGuiIO& io = ImGui::GetIO();
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        //// Setup Dear ImGui style
        //ImGui::StyleColorsDark();
        //ImGuiStyle& style = ImGui::GetStyle();
        //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //{
        //    style.WindowRounding = 0.0f;
        //    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        //}

        //// Get the SDL_Window* and GL context
        //Application& app = Application::Get();
        //SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
        //SDL_GLContext context = SDL_GL_GetCurrentContext(); 

        //// Initialize SDL3 + OpenGL3 backends
        //ImGui_ImplSDL3_InitForOpenGL(window, context);
        //ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiLayer::OnDetach()
    {
        /*ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();*/
    }

    void ImGuiLayer::Begin()
    {
        /*ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();*/
    }

    void ImGuiLayer::End()
    {
        /*ImGuiIO& io = ImGui::GetIO();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_window, backup_context);
        }*/
    }

    void ImGuiLayer::OnEvent(Event& e) {}

    void ImGuiLayer::OnImGuiRender()
    {
        /*static bool show = true;
        ImGui::ShowDemoWindow(&show);*/
    }
}