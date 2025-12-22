#pragma once

#include "Base.h"
#include "spdlog/spdlog.h"

namespace FallEngine {

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

#define FALL_CORE_TRACE(...)		::FallEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FALL_CORE_INFO(...)			::FallEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FALL_CORE_WARN(...)			::FallEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FALL_CORE_ERROR(...)		::FallEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FALL_CORE_CRITICAL(...)		::FallEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define FALL_TRACE(...)				::FallEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FALL_INFO(...)				::FallEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define FALL_WARN(...)				::FallEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FALL_ERROR(...)				::FallEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define FALL_CRITICAL(...)			::FallEngine::Log::GetClientLogger()->critical(__VA_ARGS__)