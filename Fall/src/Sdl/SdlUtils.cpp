#include "FallPCH.h"
#include "Utils/PlatformUtils.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_dialog.h>

namespace Fall {

	float Time::GetTime() {
		return static_cast<float>(SDL_GetTicks()) / 1000.0f;
	}

	static const SDL_DialogFileFilter filters[] = {
		{ "PNG images",  "png" },
		{ "JPEG images", "jpg;jpeg" },
		{ "All images",  "png;jpg;jpeg" },
		{ "All files",   "*" }
	};

	static void SDL_Callback(void* userdata, const char* const* filelist, int filter) {
		if (!filelist) {
			FALL_CORE_ERROR("File dialog error: %s", SDL_GetError());
			return;
		}
		else if (!*filelist) {
			FALL_CORE_WARN("File dialog cancelled by user.");
			return;
		}

		while (*filelist) {
			FALL_CORE_TRACE("Selected file path: '%s'", *filelist);
			filelist++;
		}

		if (filter < 0) {
			FALL_CORE_INFO("Filter index not supported or not selected.");
			return;
		}
		else if (filter < SDL_arraysize(filters)) {
			FALL_CORE_INFO("Selected filter: '%s' (%s).",
				filters[filter].pattern, filters[filter].name);
			return;
		}
	}

	void FileDialogs::OpenFile(void* window, const char* defaultPath, bool allowMany) {
		SDL_ShowOpenFileDialog(SDL_Callback,          
			nullptr,                
			static_cast<SDL_Window*>(window),
			filters,                
			SDL_arraysize(filters), 
			defaultPath,                
			allowMany);
	}

	void FileDialogs::SaveFile(void* window, const char* defaultPath) {
		SDL_ShowSaveFileDialog(SDL_Callback,           
			nullptr,                
			static_cast<SDL_Window*>(window),
			filters,                
			SDL_arraysize(filters), 
			defaultPath             
		);
	}
}