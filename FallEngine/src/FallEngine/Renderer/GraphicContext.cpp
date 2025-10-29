#include "FallEnginePCH.h"
#include "GraphicsContext.h"

#include "FallEngine/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/DirectX/DirectXContext.h"

namespace FallEngine {
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
        FALL_CORE_ASSERT(window, "GraphicsContext::Create called with nullptr window!");

        return CreateScope<VulkanContext>(static_cast<SDL_Window*>(window));
    
	}
}