#include "FallEnginePCH.h"
#include "Application.h"

#include "Window.h"
#include "FallEngine/Events/Event.h"
#include "FallEngine/Events/ApplicationEvent.h"
#include "Layer.h"

namespace FallEngine {

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        FALL_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(FALL_BIND_EVENT_FN(OnEvent));
    }

    Application::~Application() {}

    Window& Application::GetWindow() {
        return *m_Window;
    }

    void Application::PushLayer(Layer* layer) {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay) {
        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(FALL_BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
            (*--it)->OnEvent(e);
            if (e.IsHandled())
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent&) {
        m_Running = false;
        return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // --- Update layers ---
            for (Layer* layer : m_layerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

}