#include "FallEnginePCH.h"
#include "Texture.h"

namespace FallEngine {

    Texture::Texture(SDL_GPUDevice* device, const TextureSpec& spec)
        : m_Device(device), m_Spec(spec)
    {
        FALL_CORE_ASSERT(device, "Texture device is null");
        CreateTexture();
        CreateSampler();
    }

    Texture::~Texture()
    {
        if (m_Sampler)
            SDL_ReleaseGPUSampler(m_Device, m_Sampler);

        if (m_Texture)
            SDL_ReleaseGPUTexture(m_Device, m_Texture);
    }

    void Texture::CreateTexture()
    {
        SDL_GPUTextureCreateInfo info{};
        info.width = m_Spec.Width;
        info.height = m_Spec.Height;
        info.format = m_Spec.Format;

        switch (m_Spec.Usage)
        {
        case TextureUsage::Sampled:
            info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
            break;

        case TextureUsage::ColorTarget:
            info.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
            break;

        case TextureUsage::Storage:
            info.usage =
                SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_READ |
                SDL_GPU_TEXTUREUSAGE_COMPUTE_STORAGE_WRITE;
            break;
        }

        m_Texture = SDL_CreateGPUTexture(m_Device, &info);
        FALL_CORE_ASSERT(m_Texture, "Failed to create texture: {0}", SDL_GetError());
    }

    void Texture::CreateSampler()
    {
        if (m_Spec.Usage != TextureUsage::Sampled)
            return;

        SDL_GPUSamplerCreateInfo sampler{};
        sampler.min_filter =
            sampler.mag_filter =
            (m_Spec.Filter == TextureFilter::Nearest)
            ? SDL_GPU_FILTER_NEAREST
            : SDL_GPU_FILTER_LINEAR;

        sampler.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        sampler.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        m_Sampler = SDL_CreateGPUSampler(m_Device, &sampler);
        FALL_CORE_ASSERT(m_Sampler, "Failed to create sampler");
    }

}
