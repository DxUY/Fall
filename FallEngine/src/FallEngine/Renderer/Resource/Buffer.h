#pragma once

#include <cstdint>

namespace FallEngine {

	struct Buffer {
		uint32_t id = 0;
		bool IsValid() const { return id != 0; }
	};

}