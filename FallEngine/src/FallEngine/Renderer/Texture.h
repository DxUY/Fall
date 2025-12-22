#pragma once
#include <SDL3/SDL_gpu.h>
#include <cstdint>

namespace FallEngine {

    enum class TextureUsage {
        Sampled,
        ColorTarget,
        Storage
    };

    enum class TextureFilter {
        Nearest,
        Linear
    };

    struct TextureSpec {
        uint32_t Width = 1;
        uint32_t Height = 1;

        SDL_GPUTextureFormat Format;

        TextureUsage Usage = TextureUsage::Sampled;
        TextureFilter Filter = TextureFilter::Nearest;

        bool Mipmapped = false;
    };

    class Texture {
    public:
        Texture(SDL_GPUDevice* device, const TextureSpec& spec);
        ~Texture();

        SDL_GPUTexture* GetHandle() const { return m_Texture; }
        SDL_GPUSampler* GetSampler() const { return m_Sampler; }
        const TextureSpec& GetSpec() const { return m_Spec; }

    private:
        void CreateTexture();
        void CreateSampler();

    private:
        SDL_GPUDevice* m_Device = nullptr;
        SDL_GPUTexture* m_Texture = nullptr;
        SDL_GPUSampler* m_Sampler = nullptr;

        TextureSpec m_Spec;
    };

}