#include "FallPCH.h"
#include "Application.h"

#include "Layer.h"

#include "Renderer/Core/Renderer.h"

namespace Fall {

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        FALL_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(FALL_BIND_EVENT_FN(OnEvent));

        m_Renderer = CreateScope<Renderer>(m_Window->GetGPUContext());
    }

    Application::~Application() {}

    void Application::PushLayer(Layer* layer) {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        m_layerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(FALL_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
            (*it)->OnEvent(e);
            if (e.IsHandled())
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    void Application::Run() {
        while (m_Running) {
            m_Window->OnUpdate();

            m_Renderer->BeginFrame();
            
            for (Layer* layer : m_layerStack)
                layer->OnUpdate();

            m_Renderer->EndFrame();
        }
    }

}