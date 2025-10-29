#pragma once

#include "FallEngine/Core/Window.h"
#include <SDL3/SDL.h>

#include "FallEngine/Renderer/GraphicsContext.h"

namespace FallEngine {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallBack(const EventCallBackFn& callback) override { m_Data.EventCallBack = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override { return m_Data.VSync; }

		inline virtual void* GetNativeWindow() const override { return m_Window; }

		std::pair<float, float> GetMousePos() const;
		std::pair<float, float> GetMouseDelta() const;

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		void PollEvents();							 
		void ProcessEvent(const SDL_Event& event);   

	private:
		SDL_Window* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData {
			std::string Title;
			unsigned int Width = 1280;
			unsigned int Height = 720;
			bool VSync = true;

			EventCallBackFn EventCallBack;

			// Mouse state tracking
			float MouseX = 0.0f, MouseY = 0.0f;
			float LastMouseX = 0.0f, LastMouseY = 0.0f;
			float DeltaX = 0.0f, DeltaY = 0.0f;
		};

		WindowData m_Data;
	};

}
