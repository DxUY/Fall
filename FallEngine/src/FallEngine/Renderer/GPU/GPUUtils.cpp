#include "FallEnginePCH.h"
#include "GPUUtils.h"
#include "GPUCommand.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine::GPUUtils {

	void PushDebugLabel(GPUCommand& cmd, const char* name)
	{
#ifdef FALL_GRAPHICS_DEBUG
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(cmd.IsRecording(), "Debug label outside recording");
		FALL_CORE_ASSERT(name, "Null debug label");

		SDL_PushGPUCommandBufferDebugGroup(
			cmd.GetNative(),
			name
		);
#else
		(void)cmd; (void)name;
#endif
	}

	void PopDebugLabel(GPUCommand& cmd)
	{
#ifdef FALL_GRAPHICS_DEBUG
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(cmd.IsRecording(), "Debug pop outside recording");

		SDL_PopGPUCommandBufferDebugGroup(
			cmd.GetNative()
		);
#else
		(void)cmd;
#endif
	}

	DebugLabelScope::DebugLabelScope(GPUCommand& cmd, const char* name)
		: m_Command(cmd)
	{
#ifdef FALL_GRAPHICS_DEBUG
		PushDebugLabel(cmd, name);
#endif
	}

	DebugLabelScope::~DebugLabelScope()
	{
#ifdef FALL_GRAPHICS_DEBUG
		PopDebugLabel(m_Command);
#endif
	}

}