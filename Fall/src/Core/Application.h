#pragma once

#include "Base.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Core/TimeStep.h"

#include "ImGui/ImGuiLayer.h"

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

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
        Renderer& GetRenderer() { return *m_Renderer; }

        static Application& Get() { return *s_Instance; }

        Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent&);
		bool OnWindowResize(WindowResizeEvent&);

        Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

		Scope<Renderer> m_Renderer;

        static Application* s_Instance;
    };

    Application* CreateApplication();
}