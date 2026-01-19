#pragma once

#include <cstdint>

#include "FallEngine/Core/Base.h"

#include "Renderer/Resource/TextureDesc.h"
#include "Renderer/Resource/Texture.h"

struct SDL_GPUTexture;

namespace FallEngine {

    class GPUContext;

    class GPUTexture {
    public:
        GPUTexture(GPUContext& gpu, Texture handle, const TextureDesc& desc);
        ~GPUTexture();

		FALL_NON_COPYABLE(GPUTexture)

        SDL_GPUTexture* GetNative() const { return m_Native; }
        Texture GetHandle() const { return m_Handle; }

        void Resize(const TextureDesc& newDesc, uint64_t currentFrame);
        void TryDestroyDeferred(uint64_t completedFrame);

    private:
        void Create();

    private:
        GPUContext& m_GPU;
        TextureDesc m_Desc{};

        SDL_GPUTexture* m_Native = nullptr;
        SDL_GPUTexture* m_OldNative = nullptr;

        Texture  m_Handle{};
        uint64_t m_DestroyAfterFrame = UINT64_MAX;
    };

}