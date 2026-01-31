#pragma once

#include "Core/Base.h"
#include "spdlog/spdlog.h"

namespace Fall {

	class Log {
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

#define FALL_CORE_TRACE(...)		::Fall::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FALL_CORE_INFO(...)			::Fall::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FALL_CORE_WARN(...)			::Fall::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FALL_CORE_ERROR(...)		::Fall::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FALL_CORE_CRITICAL(...)		::Fall::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define FALL_TRACE(...)				::Fall::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FALL_INFO(...)				::Fall::Log::GetClientLogger()->info(__VA_ARGS__)
#define FALL_WARN(...)				::Fall::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FALL_ERROR(...)				::Fall::Log::GetClientLogger()->error(__VA_ARGS__)
#define FALL_CRITICAL(...)			::Fall::Log::GetClientLogger()->critical(__VA_ARGS__)