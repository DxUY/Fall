#pragma once

#include <SDL3/SDL_gpu.h>
#include <cstdint>

namespace FallEngine {

    enum class TextureUsage : uint32_t {
        None = 0,
        Sampled = 1 << 0,
        ColorTarget = 1 << 1,
        Storage = 1 << 2
    };

    inline TextureUsage operator|(TextureUsage a, TextureUsage b) {
        return (TextureUsage)((uint32_t)a | (uint32_t)b);
    }

    inline bool HasUsage(TextureUsage value, TextureUsage flag) {
        return ((uint32_t)value & (uint32_t)flag) != 0;
    }

    enum class TextureFilter {
        Nearest,
        Linear
    };

    struct TextureSpec {
        uint32_t Width = 1;
        uint32_t Height = 1;

        SDL_GPUTextureFormat Format =
            SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;

        TextureUsage  Usage = TextureUsage::Sampled;
        TextureFilter Filter = TextureFilter::Nearest;

        bool     Mipmapped = false;
        uint32_t MipLevels = 1;
    };

    class Texture {
    public:
        Texture(SDL_GPUDevice* device, const TextureSpec& spec);
        ~Texture();

        void Resize(uint32_t width, uint32_t height);

        SDL_GPUTexture* GetHandle()  const { return m_Texture; }
        SDL_GPUSampler* GetSampler() const { return m_Sampler; }
        const TextureSpec& GetSpec() const { return m_Spec; }

    private:
        void CreateTexture();
        void CreateSampler();
        void Release();

    private:
        SDL_GPUDevice* m_Device = nullptr;
        SDL_GPUTexture* m_Texture = nullptr;
        SDL_GPUSampler* m_Sampler = nullptr;
        TextureSpec m_Spec;
    };

}