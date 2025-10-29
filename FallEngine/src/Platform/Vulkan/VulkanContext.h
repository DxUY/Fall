#pragma once

#include "FallEngine/Renderer/GraphicsContext.h"

class SDL_Window;

namespace FallEngine{
	class VulkanContext : public GraphicsContext 
	{
	public:
		VulkanContext(SDL_Window* windowHandler);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		SDL_Window* m_WindowHandler;
	};
}