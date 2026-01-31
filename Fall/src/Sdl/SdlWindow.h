#pragma once

#include "Core/Window.h"

#include "SDL3/SDL.h"

struct SDL_Window;

namespace Fall {

	class GPUContext;

	class SdlWindow : public Window {
	public:
		SdlWindow(const WindowProps& props);
		virtual ~SdlWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallBack = callback; }
		void SetVSync(bool enabled) override { m_Data.VSync = enabled; }
		bool IsVSync() const override { return m_Data.VSync; }

		inline virtual void* GetNativeWindow() const override { return m_Window; }

		std::pair<float, float> GetMousePos() const;
		std::pair<float, float> GetMouseDelta() const;

		GPUContext& GetGPUContext() { return *m_GPUContext; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		void PollEvents();
		void ProcessEvent(const SDL_Event& event);

	private:
		SDL_Window* m_Window;
		Scope<GPUContext> m_GPUContext;

		struct WindowData {
			std::string Title;
			unsigned int Width = 1280;
			unsigned int Height = 720;
			bool VSync = true;

			EventCallbackFn EventCallBack;

			float MouseX = 0.0f, MouseY = 0.0f;
			float LastMouseX = 0.0f, LastMouseY = 0.0f;
			float DeltaX = 0.0f, DeltaY = 0.0f;
		};

		WindowData m_Data;
	};

}