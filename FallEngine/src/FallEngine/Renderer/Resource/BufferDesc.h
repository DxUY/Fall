#pragma once

#include <cstdint>

namespace FallEngine {

	enum class BufferUsage : uint32_t {
		None = 0,
		Vertex = 1 << 0,
		Index = 1 << 1,
		Indirect = 1 << 2,
		GraphicsRead = 1 << 3,
		ComputeRead = 1 << 4,
		ComputeWrite = 1 << 5,
	};

	inline BufferUsage operator|(BufferUsage a, BufferUsage b)
	{
		return static_cast<BufferUsage>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
			);
	}

	struct BufferDesc {
		uint64_t size = 0;
		BufferUsage usage = BufferUsage::None;
	};

}