#include "FallEnginePCH.h"
#include "GPUUtils.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine::GPUUtils {

	void PushDebugLabel(const char* name)
	{
#ifdef FALL_GRAPHICS_DEBUG
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(name, "GPU debug label name is null");

		SDL_PushGPUCommandDebugLabel(name);
#endif
	}

	void PopDebugLabel()
	{
#ifdef FALL_GRAPHICS_DEBUG
		FALL_ASSERT_GPU_THREAD();

		SDL_PopGPUCommandDebugLabel();
#endif
	}

}