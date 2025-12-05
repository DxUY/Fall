#pragma once

namespace FallEngine {
	class FileDialogs {
	public:
		void OpenFile(void* window = nullptr, const char* defaultPath = nullptr, bool allowMany = false);
		void SaveFile(void* window = nullptr, const char* defaultPath = nullptr);
	};

	class Time {
	public:
		static float GetTime();
	};

}