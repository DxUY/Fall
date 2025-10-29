#pragma once

#include "FallEngine/Renderer/GraphicsContext.h"

class SDL_Window;

namespace FallEngine {
	class DirectXContext : public GraphicsContext
	{
	public:
		DirectXContext(SDL_Window* windowHandler);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		SDL_Window* m_WindowHandler;
	};
}