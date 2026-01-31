#pragma once

#include "Core/Base.h"
#include "Renderer/Resource/Texture/TextureDesc.h"

struct SDL_GPUTexture;

namespace Fall {

    class GPUContext;

    class GPUTexture {
    public:
        GPUTexture(GPUContext& gpu, const TextureDesc& desc);
        ~GPUTexture();

        FALL_NON_COPYABLE(GPUTexture)

        SDL_GPUTexture* GetNative() const { return m_Native; }
        const TextureDesc& GetDesc() const { return m_Desc; }

        void Resize(const TextureDesc& newDesc);

    private:
        void Create();

    private:
        GPUContext& m_GPU;
        TextureDesc m_Desc{};
        SDL_GPUTexture* m_Native = nullptr;
    };

}