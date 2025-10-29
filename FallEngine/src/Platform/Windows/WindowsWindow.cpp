#include "WindowsWindow.h"

#include "FallEngine/Events/ApplicationEvent.h"
#include "FallEngine/Events/KeyEvent.h"
#include "FallEngine/Events/MouseEvent.h"
#include <SDL3/SDL.h>

namespace FallEngine {

	static bool s_SDLInitialized = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.VSync = true;

		// Reset mouse state
		m_Data.MouseX = m_Data.MouseY = 0.0f;
		m_Data.LastMouseX = m_Data.LastMouseY = 0.0f;
		m_Data.DeltaX = m_Data.DeltaY = 0.0f;

		if (!s_SDLInitialized) {
			int result = SDL_Init(SDL_INIT_VIDEO);
			FALL_CORE_ASSERT(result == 0, "Failed to initialize SDL: {0}", SDL_GetError());
			s_SDLInitialized = true;
		}

		m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		FALL_CORE_ASSERT(m_Window != nullptr, "Failed to create SDL window: {0}", SDL_GetError());

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		
	}

	void WindowsWindow::PollEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			/*ImGui_ImplSDL3_ProcessEvent(&event);*/
			ProcessEvent(event);
		}
	}

	void WindowsWindow::ProcessEvent(const SDL_Event& event) {
		switch (event.type) {
		case SDL_EVENT_QUIT: {
			if (m_Data.EventCallBack) {
				FALL_CORE_INFO("SDL_EVENT_QUIT received");
				WindowCloseEvent e;
				m_Data.EventCallBack(e);
			}
			break;
		}
		case SDL_EVENT_WINDOW_RESIZED: {
			if (m_Data.EventCallBack) {
				FALL_CORE_INFO("SDL_EVENT_RESIZE received");
				WindowResizeEvent e(event.window.data1, event.window.data2);
				m_Data.EventCallBack(e);
			}
			break;
		}
		case SDL_EVENT_KEY_DOWN: {
			if (m_Data.EventCallBack) {
				KeyPressedEvent e(event.key.key, event.key.repeat);
				m_Data.EventCallBack(e);
			}
			break;
		}
		case SDL_EVENT_KEY_UP: {
			if (m_Data.EventCallBack) {
				KeyReleasedEvent e(event.key.key);
				m_Data.EventCallBack(e);
			}
			break;
		}
		case SDL_EVENT_MOUSE_MOTION: {
			if (m_Data.EventCallBack) {
				float x = static_cast<float>(event.motion.x);
				float y = static_cast<float>(event.motion.y);

				m_Data.DeltaX = x - m_Data.MouseX;
				m_Data.DeltaY = y - m_Data.MouseY;

				m_Data.LastMouseX = m_Data.MouseX;
				m_Data.LastMouseY = m_Data.MouseY;
				m_Data.MouseX = x;
				m_Data.MouseY = y;

				MouseMovedEvent e(x, y, m_Data.DeltaX, m_Data.DeltaY);
				m_Data.EventCallBack(e);
			}
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_DOWN: {
			if (m_Data.EventCallBack) {
				MouseButtonPressedEvent e(event.button.button);
				m_Data.EventCallBack(e);
			}
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			if (m_Data.EventCallBack) {
				MouseButtonReleasedEvent e(event.button.button);
				m_Data.EventCallBack(e);
			}
			break;
		}
		case SDL_EVENT_MOUSE_WHEEL: {
			if (m_Data.EventCallBack) {
				MouseScrolledEvent e(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
				m_Data.EventCallBack(e);
			}
			break;
		}
		default:
			break;
		}
	}

	void WindowsWindow::OnUpdate() {
		m_Data.DeltaX = 0.0f;
		m_Data.DeltaY = 0.0f;

		PollEvents();            
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled) {
		
	}

	std::pair<float, float> WindowsWindow::GetMousePos() const {
		return { m_Data.MouseX, m_Data.MouseY };
	}

	std::pair<float, float> WindowsWindow::GetMouseDelta() const {
		return { m_Data.DeltaX, m_Data.DeltaY };
	}

}