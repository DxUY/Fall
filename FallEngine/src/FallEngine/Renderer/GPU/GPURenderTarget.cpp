#include "FallEnginePCH.h"

#include "GPURenderTarget.h"
#include "GPUContext.h"
#include "GPUCommand.h"

namespace FallEngine {

	GPURenderTarget::GPURenderTarget(
		GPUContext& gpu,
		GPUCommand& cmd,
		const RenderTarget& desc
	) {
		SDL_GPUTexture* texture = nullptr;
		uint32_t w = 0, h = 0;

		if (desc.IsBackbuffer()) {
			gpu.AcquireBackbuffer(
				cmd.GetNativeCommandBuffer(),
				&texture,
				&w,
				&h
			);
		}

		m_ColorInfo.texture = texture;
		m_ColorInfo.load_op =
			desc.GetLoadOp() == LoadOp::Clear
			? SDL_GPU_LOADOP_CLEAR
			: SDL_GPU_LOADOP_LOAD;

		m_ColorInfo.store_op =
			desc.GetStoreOp() == StoreOp::Store
			? SDL_GPU_STOREOP_STORE
			: SDL_GPU_STOREOP_DONT_CARE;

		auto& c = desc.GetClearColor();
		m_ColorInfo.clear_color = { c.r, c.g, c.b, c.a };
	}

}