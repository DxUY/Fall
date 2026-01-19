#include "SdlWindow.h"

#include "FallEngine/Core/Input.h"

#include "FallEngine/Events/ApplicationEvent.h"
#include "FallEngine/Events/KeyEvent.h"
#include "FallEngine/Events/MouseEvent.h"

#include "FallEngine/Renderer/GPU/GPUContext.h"

namespace FallEngine {

	static uint8_t s_SDLWindowCount = 0;

	Window* Window::Create(const WindowProps& props) {
		return new SdlWindow(props);
	}

	SdlWindow::SdlWindow(const WindowProps& props) {
		Init(props);
	}

	SdlWindow::~SdlWindow() {
		Shutdown();
		m_GPUContext->Shutdown();
	}

	void SdlWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.VSync = true;

		// Reset mouse state
		m_Data.MouseX = m_Data.MouseY = 0.0f;
		m_Data.LastMouseX = m_Data.LastMouseY = 0.0f;
		m_Data.DeltaX = m_Data.DeltaY = 0.0f;

		if (s_SDLWindowCount == 0) {
			int result = SDL_Init(SDL_INIT_VIDEO);
			FALL_CORE_ASSERT(result == 0, "Failed to initialize SDL: {0}", SDL_GetError());
		}

		{
#if defined(FALL_DEBUG)

#endif
			m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, SDL_WINDOW_RESIZABLE);
			++s_SDLWindowCount;
		}

		m_GPUContext = CreateScope<GPUContext>();
		m_GPUContext->Initialize(m_Window);

		SetVSync(true);
	}

	void SdlWindow::Shutdown() {
		SDL_DestroyWindow(m_Window);
		--s_SDLWindowCount;

		if (s_SDLWindowCount == 0) SDL_Quit();
	}

	void SdlWindow::PollEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			/*ImGui_ImplSDL3_ProcessEvent(&event);*/
			ProcessEvent(event);
		}
	}

	void SdlWindow::ProcessEvent(const SDL_Event& event) {
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

	void SdlWindow::OnUpdate() {
		m_Data.DeltaX = 0.0f;
		m_Data.DeltaY = 0.0f;

		PollEvents();
	}

}