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

    void Application::PushLayer(Scope<Layer> layer) {
        Layer* raw = layer.get();
        m_layerStack.PushLayer(std::move(layer));
        raw->OnAttach();
    }

    void Application::PushOverlay(Scope<Layer> overlay) {
        Layer* raw = overlay.get();
        m_layerStack.PushOverlay(std::move(overlay));
        raw->OnAttach();
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(FALL_BIND_EVENT_FN(OnWindowClose));

        auto& layers = m_layerStack;
        std::vector<Layer*> rawPtrs;
        for (auto it = m_layerStack.begin(); it != m_layerStack.end(); ++it) rawPtrs.push_back(*it);

        for (auto it = rawPtrs.rbegin(); it != rawPtrs.rend(); ++it) {
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

            for (auto it = m_layerStack.begin(); it != m_layerStack.end(); ++it)
                (*it)->OnUpdate();

            m_Window->OnUpdate();
        }
    }

}