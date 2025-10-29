#include "FallEnginePCH.h"
#include "DirectXContext.h"



namespace FallEngine {
	DirectXContext::DirectXContext(SDL_Window* windowHandler)
		: m_WindowHandler(windowHandler)
	{
		FALL_CORE_ASSERT(windowHandler, "Window handler is null!");
	}

	void DirectXContext::Init()
	{
		
	}

	void DirectXContext::SwapBuffers()
	{
		FALL_CORE_ASSERT(m_WindowHandler, "SwapBuffers called with null window handler!");
	}
}