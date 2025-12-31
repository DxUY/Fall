#pragma once

namespace FallEngine::GPUUtils {

	// Push a GPU debug label onto the current command buffer
	void PushDebugLabel(const char* name);

	// Pop the most recent GPU debug label
	void PopDebugLabel();

	// RAII helper for GPU debug scopes
	class DebugLabelScope {
	public:
		explicit DebugLabelScope(const char* name)
		{
#ifdef FALL_GRAPHICS_DEBUG
			PushDebugLabel(name);
#endif
		}

		~DebugLabelScope()
		{
#ifdef FALL_GRAPHICS_DEBUG
			PopDebugLabel();
#endif
		}
	};

}