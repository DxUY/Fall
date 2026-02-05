#pragma once

#include "Core/PlatformDetection.h"

#ifdef FALL_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#define WIN32_LEAN_AND_MEAN
#endif

#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Core/Base.h"

#include "DebugTools/Log.h"

#include "DebugTools/Instrumentor.h"

#ifdef FALL_PLATFORM_WINDOWS
	#include <Windows.h>

	#ifdef min
		#undef min
	#endif
	#ifdef max
		#undef max
	#endif
#endif