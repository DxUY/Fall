#pragma once

#include "FallEngine/Core/Base.h"
#include <type_traits>
#include <cstddef>
#include <cstdint>

namespace FallEngine {

	class GPUCommand;
	class RenderTarget;
	struct BackbufferView;

	class GPURenderTarget {
	public:
		GPURenderTarget(
			GPUCommand& cmd,
			const BackbufferView& backbuffer, 
			const RenderTarget& desc
		);

		FALL_NON_COPYABLE(GPURenderTarget)

		const void* GetNativeInfo() const;

	private:
		alignas(std::max_align_t) std::byte m_Storage[128];
	};

}