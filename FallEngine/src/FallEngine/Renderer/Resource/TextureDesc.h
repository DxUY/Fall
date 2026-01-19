#pragma once
#include <cstdint>

namespace FallEngine {

	enum class TextureUsage : uint32_t {
		None = 0,
		Sampler = 1 << 0,
		ColorTarget = 1 << 1,
		DepthStencilTarget = 1 << 2,
		GraphicsStorageRead = 1 << 3,
		ComputeStorageRead = 1 << 4,
		ComputeStorageWrite = 1 << 5,
		ComputeStorageReadWrite = 1 << 6
	};

	inline TextureUsage operator|(TextureUsage a, TextureUsage b)
	{
		return static_cast<TextureUsage>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
			);
	}

	enum class TextureFormat : uint32_t {
		Unknown = 0,

		RGBA8_UNorm,
		BGRA8_UNorm,
		RGBA16_Float,
		R32_Float,

		D32_Float,
		D24S8
	};

	struct TextureDesc {
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t layers = 1;
		uint32_t mipLevels = 1;
		uint32_t samples = 1;

		TextureFormat format = TextureFormat::Unknown;
		TextureUsage  usage = TextureUsage::None;

		bool IsValid() const
		{
			return width > 0 && height > 0 && format != TextureFormat::Unknown;
		}
	};

}