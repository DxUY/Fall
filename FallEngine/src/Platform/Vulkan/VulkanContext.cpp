#include "FallEnginePCH.h"
#include "VulkanContext.h"

namespace FallEngine {

	VulkanContext::VulkanContext(SDL_Window* windowHandler)
		: m_WindowHandler(windowHandler)
	{
		FALL_CORE_ASSERT(m_WindowHandler, "Window handler is null!");
	}

    void VulkanContext::Init()
    {
		FALL_CORE_ASSERT(m_WindowHandler, "Init called with null window handler!");
		FALL_CORE_INFO("VulkanContext::Init() called successfully on window");
    }

	void VulkanContext::SwapBuffers()
	{
		FALL_CORE_ASSERT(m_WindowHandler, "SwapBuffers called with null window handler!");
	}
}