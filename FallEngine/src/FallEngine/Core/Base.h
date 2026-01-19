#pragma once

#include <memory>
#include <thread>

#ifdef FALL_DEBUG
    #if defined(FALL_PLATFORM_WINDOWS)
        #define FALL_DEBUGBREAK() __debugbreak()
    #elif defined(FALL_PLATFORM_LINUX)
        #include <signal.h>
        #define FALL_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
#else
    #define FALL_DEBUGBREAK()
#endif

#ifdef FALL_DEBUG
	#define FALL_GRAPHICS_DEBUG
#endif

#ifdef FALL_DEBUG
	#define FALL_ASSERT_GPU_THREAD()                                     \
			do {                                                             \
				if (!::FallEngine::IsOnGPUThread()) {                        \
					FALL_CORE_ERROR("GPU Thread Violation");                 \
					FALL_CORE_ERROR("Expected GPU thread");                  \
					FALL_DEBUGBREAK();                                       \
				}                                                            \
			} while (0)
#else
	#define FALL_ASSERT_GPU_THREAD()
#endif


#define FALL_EXPAND_MACRO(x) x
#define FALL_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define FALL_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define FALL_NON_COPYABLE(T)            \
    T(const T&) = delete;               \
    T& operator=(const T&) = delete;    \
    T(T&&) = delete;                    \
    T& operator=(T&&) = delete;

namespace FallEngine {

	void SetGPUThreadID(bool force = false);
	bool IsOnGPUThread();

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "FallEngine/DebugTools/Log.h"
#include "FallEngine/DebugTools/Assert.h"