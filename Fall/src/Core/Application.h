#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace Fall {

    class Window;
    class Event;
    class WindowCloseEvent;

    class Renderer;

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        static Application& Get() { return *s_Instance; }
        Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_layerStack;

		Scope<Renderer> m_Renderer;

        static Application* s_Instance;
    };

    Application* CreateApplication();
}