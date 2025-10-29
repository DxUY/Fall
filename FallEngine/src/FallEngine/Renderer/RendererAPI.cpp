#include "FallEnginePCH.h"
#include "RendererAPI.h"

#include "Platform/Vulkan/VulkanRendererAPI.h"

namespace FallEngine {
	
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    FALL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Direct12:  FALL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:   return CreateScope<VulkanRendererAPI>();
		}

		FALL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}