#include "FallPCH.h"
#include "GPURenderTarget.h"

#include <SDL3/SDL_gpu.h>

#include "GPUContext.h"
#include "GPUTexture.h"

#include "Renderer/Resource/Target/RenderTarget.h"

namespace Fall {

    GPURenderTarget::GPURenderTarget(const BackbufferView& backbuffer, const RenderTarget& desc) 
        : m_Storage{} {
        FALL_ASSERT_GPU_THREAD();

        auto* info = reinterpret_cast<SDL_GPUColorTargetInfo*>(m_Storage);
        *info = {};

        if (desc.IsBackbuffer()) {
            FALL_CORE_ASSERT(backbuffer.IsValid(), "BackbufferView is invalid!");
            info->texture = static_cast<SDL_GPUTexture*>(backbuffer.opaque);

            FALL_CORE_ASSERT(
                info->texture,
                "Attempting to use Backbuffer, but native opaque pointer is null!"
            );
        }
        else if (desc.HasTexture()) {
            GPUTexture* texture = desc.GetTexture();
            FALL_CORE_ASSERT(texture, "RenderTarget reports Texture type but pointer is null!");

            info->texture = texture->GetNative();
            FALL_CORE_ASSERT(info->texture, "GPUTexture native handle is null!");
        }
        else {
            FALL_CORE_ASSERT(false, "Unknown or uninitialized RenderTarget type!");
        }

        info->load_op = (desc.GetLoadOp() == LoadOp::Clear)
            ? SDL_GPU_LOADOP_CLEAR
            : SDL_GPU_LOADOP_LOAD;

        info->store_op = (desc.GetStoreOp() == StoreOp::Store)
            ? SDL_GPU_STOREOP_STORE
            : SDL_GPU_STOREOP_DONT_CARE;

        const auto& c = desc.GetClearColor();
        info->clear_color = { c.r, c.g, c.b, c.a };

        info->mip_level = 0;
        info->layer_or_depth_plane = 0;
        info->cycle = false;
    }

    const void* GPURenderTarget::GetNativeInfo() const {
        return m_Storage;
    }

}