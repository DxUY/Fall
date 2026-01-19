#include "FallEnginePCH.h"
#include "GPURenderTarget.h"

#include <SDL3/SDL_gpu.h>

#include "GPUContext.h"
#include "GPUCommand.h"
#include "GPUTexture.h"

#include "Renderer/Target/RenderTarget.h"

namespace FallEngine {

	GPURenderTarget::GPURenderTarget(
		GPUCommand& cmd,
		const BackbufferView& backbuffer,
		const RenderTarget& desc) : m_Storage{} {
		FALL_ASSERT_GPU_THREAD();

		auto* info = reinterpret_cast<SDL_GPUColorTargetInfo*>(m_Storage);
		*info = {};

		if (desc.IsBackbuffer()) {
			info->texture = static_cast<SDL_GPUTexture*>(backbuffer.opaque);
			FALL_CORE_ASSERT(info->texture, "Attempting to use Backbuffer as target but it is null!");
		}
		else if (desc.HasTexture()) {
			GPUTexture* tex = cmd.GetGPUContext().GetTextureRegistry().Resolve(desc.GetTexture());

			FALL_CORE_ASSERT(tex, "Failed to resolve Texture handle in RenderTarget");
			info->texture = tex->GetNative();
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

	const void* GPURenderTarget::GetNativeInfo() const
	{
		return m_Storage;
	}

}