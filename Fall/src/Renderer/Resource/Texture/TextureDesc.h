#pragma once

#include <cstdint>

namespace Fall {

    enum class TextureFormat : uint8_t {
        RGBA8_UNorm,
        BGRA8_UNorm,
        RGBA16_Float,
        R32_Float,
        D32_Float,
        D24S8
    };

    enum class TextureUsage : uint32_t {
        None = 0,
        Sampled = 1 << 0,
        Storage = 1 << 1,
        RenderTarget = 1 << 2,
        DepthStencil = 1 << 3,
        TransferSrc = 1 << 4,
        TransferDst = 1 << 5
    };

    struct TextureDesc {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t layers = 1;
        uint32_t mipLevels = 1;
        uint32_t samples = 1;

        TextureFormat format = TextureFormat::RGBA8_UNorm;
        TextureUsage  usage = TextureUsage::None;

        bool IsValid() const {
            return width > 0 && height > 0;
        }

        bool operator==(const TextureDesc& rhs) const {
            return width == rhs.width &&
                height == rhs.height &&
                layers == rhs.layers &&
                mipLevels == rhs.mipLevels &&
                samples == rhs.samples &&
                format == rhs.format &&
                usage == rhs.usage;
        }

        bool operator!=(const TextureDesc& rhs) const {
            return !(*this == rhs);
        }
    };

}