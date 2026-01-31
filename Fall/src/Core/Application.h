#pragma once

#include "Base.h"
#include "LayerStack.h"

namespace Fall {

    class Window;
    class Event;
    class WindowCloseEvent;

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Scope<Layer> layer);
        void PushOverlay(Scope<Layer> overlay);

        static Application& Get() { return *s_Instance; }
        Window& GetWindow();

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_layerStack;

        static Application* s_Instance;
    };

    Application* CreateApplication();
}