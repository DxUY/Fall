#include "FallPCH.h"
#include "Application.h"

#include "Renderer/Core/Renderer.h"

#include "Utils/PlatformUtils.h"

namespace Fall {
    Application* Application::s_Instance = nullptr;

    Application::Application() {
        FALL_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        SetGPUThreadID();

        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(FALL_BIND_EVENT_FN(OnEvent));

        m_Renderer = CreateScope<Renderer>(m_Window->GetGPUContext());

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {}

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(FALL_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            (*it)->OnEvent(e);
            if (e.IsHandled())
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent&) {
        m_Running = false;
        return true;
    }

    void Application::Run() {
        while (m_Running) {
            F_PROFILE_SCOPE("RunLoop");

            float time = Time::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_Window->OnUpdate();

            m_Renderer->BeginFrame();
            
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            {
                F_PROFILE_SCOPE("LayerStack OnImGuiRender");

                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Renderer->EndFrame();
        }
    }

}