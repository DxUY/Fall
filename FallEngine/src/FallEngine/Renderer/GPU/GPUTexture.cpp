#include "FallEnginePCH.h"

#include "GPUTexture.h"

#include "GPUContext.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

    inline SDL_GPUTextureUsageFlags ToSDLUsages(TextureUsage usage) {
        return static_cast<SDL_GPUTextureUsageFlags>(static_cast<uint32_t>(usage));
    }

    static SDL_GPUTextureFormat ToSDLFormats(TextureFormat format) {
        switch (format) {
        case TextureFormat::RGBA8_UNorm:   return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        case TextureFormat::BGRA8_UNorm:   return SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM;
        case TextureFormat::RGBA16_Float:  return SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT;
        case TextureFormat::R32_Float:     return SDL_GPU_TEXTUREFORMAT_R32_FLOAT;
        case TextureFormat::D32_Float:     return SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
        case TextureFormat::D24S8:         return SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT;
        default:                           return SDL_GPU_TEXTUREFORMAT_INVALID;
        }
    }

    GPUTexture::GPUTexture(GPUContext& gpu, Texture handle, const TextureDesc& desc)
        : m_GPU(gpu), m_Handle(handle), m_Desc(desc) {
        FALL_CORE_ASSERT(desc.IsValid(), "Invalid TextureDesc");
        FALL_CORE_ASSERT(handle.IsValid(), "Invalid Texture handle passed to GPUTexture");

        Create();

        m_GPU.GetTextureRegistry().Register(m_Handle, this);
    }

    GPUTexture::~GPUTexture() {
        m_GPU.GetTextureRegistry().Remove(m_Handle);

        if (m_Native) {
            SDL_ReleaseGPUTexture(m_GPU.GetDevice(), m_Native);
            m_Native = nullptr;
        }

        if (m_OldNative) {
            SDL_ReleaseGPUTexture(m_GPU.GetDevice(), m_OldNative);
            m_OldNative = nullptr;
        }
    }

    void GPUTexture::Create() {
        FALL_ASSERT_GPU_THREAD();

        SDL_GPUTextureCreateInfo info{};
        info.type = SDL_GPU_TEXTURETYPE_2D; // Standard default
        info.width = m_Desc.width;
        info.height = m_Desc.height;
        info.layer_count_or_depth = m_Desc.layers;
        info.num_levels = m_Desc.mipLevels;
        info.sample_count = static_cast<SDL_GPUSampleCount>(m_Desc.samples);
        info.format = ToSDLFormats(m_Desc.format);
        info.usage = ToSDLUsages(m_Desc.usage);

        FALL_CORE_ASSERT(info.format != SDL_GPU_TEXTUREFORMAT_INVALID, "Unsupported texture format");

        m_Native = SDL_CreateGPUTexture(m_GPU.GetDevice(), &info);
        FALL_CORE_ASSERT(m_Native, "Failed to create SDL_GPUTexture");
    }

    void GPUTexture::Resize(const TextureDesc& newDesc, uint64_t currentFrame) {
        FALL_ASSERT_GPU_THREAD();

        if (newDesc.width == m_Desc.width &&
            newDesc.height == m_Desc.height &&
            newDesc.format == m_Desc.format &&
            newDesc.usage == m_Desc.usage)
            return;

        if (m_OldNative) {
            SDL_ReleaseGPUTexture(m_GPU.GetDevice(), m_OldNative);
        }

        m_OldNative = m_Native;
        m_DestroyAfterFrame = currentFrame + 3;

        m_Desc = newDesc;
        m_Native = nullptr;
        Create();
    }

    void GPUTexture::TryDestroyDeferred(uint64_t completedFrame) {
        FALL_ASSERT_GPU_THREAD();

        if (m_OldNative && completedFrame >= m_DestroyAfterFrame) {
            SDL_ReleaseGPUTexture(m_GPU.GetDevice(), m_OldNative);
            m_OldNative = nullptr;
            m_DestroyAfterFrame = UINT64_MAX;
        }
    }
}