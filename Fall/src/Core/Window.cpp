#include "FallPCH.h"
#include "Window.h"

#ifdef FALL_PLATFORM_WINDOWS
#include "Sdl/SdlWindow.h"
#endif

namespace Fall
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef FALL_PLATFORM_WINDOWS
		return CreateScope<SdlWindow>(props);
#else
		FALL_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}