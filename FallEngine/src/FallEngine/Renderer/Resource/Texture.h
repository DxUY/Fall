#pragma once

#include <cstdint>

namespace FallEngine {

	struct Texture {
		uint32_t id = 0;
		bool IsValid() const { return id != 0; }
	};

}