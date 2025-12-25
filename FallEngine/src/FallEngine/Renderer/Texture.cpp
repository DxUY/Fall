#include "FallEnginePCH.h"
#include "Texture.h"

namespace FallEngine {

    Texture::Texture(SDL_GPUDevice* device, const TextureSpec& spec) 
        : m_Device(device), m_Spec(spec) {
        FALL_CORE_ASSERT(m_Device, "Texture device is null");

        m_Spec.Width = m_Spec.Width > 0 ? m_Spec.Width : 1;
        m_Spec.Height = m_Spec.Height > 0 ? m_Spec.Height : 1;

        CreateTexture();
        CreateSampler();
    }

    Texture::~Texture() {
        Release();
    }

    void Texture::Release() {
        if (m_Sampler) {
            SDL_ReleaseGPUSampler(m_Device, m_Sampler);
            m_Sampler = nullptr;
        }

        if (m_Texture) {
            SDL_ReleaseGPUTexture(m_Device, m_Texture);
            m_Texture = nullptr;
        }
    }

    void Texture::Resize(uint32_t width, uint32_t height) {
        width = width > 0 ? width : 1;
        height = height > 0 ? height : 1;

        if (m_Spec.Width == width && m_Spec.Height == height)
            return;

        m_Spec.Width = width;
        m_Spec.Height = height;

        Release();
        CreateTexture();
        CreateSampler();
    }

    void Texture::CreateTexture() {
        FALL_CORE_INFO("Creating texture: {0}x{1}, format={2}, usage={3}",
            m_Spec.Width, m_Spec.Height,
            (uint32_t)m_Spec.Format,
            (uint32_t)m_Spec.Usage);

        SDL_GPUTextureCreateInfo info{};
        info.type = SDL_GPU_TEXTURETYPE_2D;
        info.format = m_Spec.Format;

        info.width = m_Spec.Width;
        info.height = m_Spec.Height;
        info.layer_count_or_depth = 1;
		info.num_levels = m_Spec.Mipmapped ? m_Spec.MipLevels : 1;
        info.sample_count = SDL_GPU_SAMPLECOUNT_1;

        info.usage = 0;

        if (HasUsage(m_Spec.Usage, TextureUsage::Sampled))
            info.usage |= SDL_GPU_TEXTUREUSAGE_SAMPLER;

        if (HasUsage(m_Spec.Usage, TextureUsage::ColorTarget))
            info.usage |= SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;

        if (HasUsage(m_Spec.Usage, TextureUsage::Storage)) {
            info.usage |= SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ;
            info.usage |= SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE;
        }

        FALL_CORE_ASSERT(info.usage != 0,
            "Texture usage must not be empty");

        m_Texture = SDL_CreateGPUTexture(m_Device, &info);
        FALL_CORE_ASSERT(m_Texture,
            "SDL_CreateGPUTexture failed: {0}", SDL_GetError());
    }

    void Texture::CreateSampler() {
        if (!HasUsage(m_Spec.Usage, TextureUsage::Sampled))
            return;

        SDL_GPUSamplerCreateInfo sampler{};
        sampler.min_filter =
            sampler.mag_filter =
            (m_Spec.Filter == TextureFilter::Nearest)
            ? SDL_GPU_FILTER_NEAREST
            : SDL_GPU_FILTER_LINEAR;

        sampler.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        sampler.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        sampler.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        m_Sampler = SDL_CreateGPUSampler(m_Device, &sampler);
        FALL_CORE_ASSERT(m_Sampler,
            "SDL_CreateGPUSampler failed");
    }

}
