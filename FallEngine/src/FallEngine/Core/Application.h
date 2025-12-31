#pragma once

#include "Base.h"
#include "LayerStack.h"

#include "Renderer/GPU/GPUContext.h"
#include "Renderer/Core/Renderer.h"

namespace FallEngine {

    class Window;
    class Event;
    class WindowCloseEvent;

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        static Application& Get() { return *s_Instance; }
        Window& GetWindow();

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        Scope<Window> m_Window;

        Scope<GPUContext> m_GPUContext;
        Scope<Renderer>   m_Renderer;
        
        bool m_Running = true;
        LayerStack m_layerStack;

        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();

}
