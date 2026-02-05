#include "FallPCH.h"
#include "GPUTexture.h"

#include "Renderer/GPU/GPUContext.h"
#include <SDL3/SDL_gpu.h>

namespace Fall {

    static SDL_GPUTextureUsageFlags ToSDLUsage(TextureUsage usage) {
        return static_cast<SDL_GPUTextureUsageFlags>(
            static_cast<uint32_t>(usage)
            );
    }

    static SDL_GPUTextureFormat ToSDLFormat(TextureFormat format) {
        switch (format) {
        case TextureFormat::RGBA8_UNorm:  return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        case TextureFormat::BGRA8_UNorm:  return SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM;
        case TextureFormat::RGBA16_Float: return SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT;
        case TextureFormat::R32_Float:    return SDL_GPU_TEXTUREFORMAT_R32_FLOAT;
        case TextureFormat::D32_Float:    return SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
        case TextureFormat::D24S8:        return SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT;
        default:                          return SDL_GPU_TEXTUREFORMAT_INVALID;
        }
    }

    GPUTexture::GPUTexture(GPUContext& context, const TextureDesc& desc)
        : m_Context(context), m_Desc(desc) {

        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(desc.IsValid(), "Invalid TextureDesc");

        Create();
    }

    GPUTexture::~GPUTexture() {
        FALL_ASSERT_GPU_THREAD();

        if (m_Native) {
            SDL_ReleaseGPUTexture(m_Context.GetDevice(), m_Native);
            m_Native = nullptr;
        }
    }

    void GPUTexture::Create() {
        FALL_ASSERT_GPU_THREAD();

        SDL_GPUTextureCreateInfo info{};
        info.type = SDL_GPU_TEXTURETYPE_2D;
        info.width = m_Desc.width;
        info.height = m_Desc.height;
        info.layer_count_or_depth = m_Desc.layers;
        info.num_levels = m_Desc.mipLevels;
        info.sample_count = static_cast<SDL_GPUSampleCount>(m_Desc.samples);
        info.format = ToSDLFormat(m_Desc.format);
        info.usage = ToSDLUsage(m_Desc.usage);

        FALL_CORE_ASSERT(info.format != SDL_GPU_TEXTUREFORMAT_INVALID,
            "Unsupported texture format");

        m_Native = SDL_CreateGPUTexture(m_Context.GetDevice(), &info);
        FALL_CORE_ASSERT(m_Native, "Failed to create GPU texture");
    }

    void GPUTexture::Resize(const TextureDesc& newDesc) {
        FALL_ASSERT_GPU_THREAD();
        FALL_CORE_ASSERT(newDesc.IsValid(), "Invalid TextureDesc");

        if (newDesc == m_Desc)
            return;

        if (m_Native) {
            SDL_ReleaseGPUTexture(m_Context.GetDevice(), m_Native);
            m_Native = nullptr;
        }

        m_Desc = newDesc;
        Create();
    }

}