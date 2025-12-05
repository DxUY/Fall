#pragma once

#ifdef FALL_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <limits>

#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "FallEngine/Core/Base.h"

#ifdef FALL_PLATFORM_WINDOWS
	#include <Windows.h>

	#ifdef min
		#undef min
	#endif
	#ifdef max
		#undef max
	#endif
#endif