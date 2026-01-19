#include "FallEnginePCH.h"
#include "GPUComputePass.h"

#include "GPUCommand.h"
#include "GPUContext.h"
#include "GPUBuffer.h"
#include "GPUTexture.h"

#include <SDL3/SDL_gpu.h>

namespace FallEngine {

	GPUComputePass::GPUComputePass(
		GPUCommand& cmd,
		const std::vector<Buffer>& storageBuffers,
		const std::vector<Texture>& storageTextures)
		: m_Cmd(cmd)
	{
		FALL_ASSERT_GPU_THREAD();

		std::vector<SDL_GPUStorageBufferReadWriteBinding> bufferBindings;
		bufferBindings.reserve(storageBuffers.size());
		for (const auto& handle : storageBuffers) {
			auto* gpuBuf = m_Cmd.GetGPUContext().GetBufferRegistry().Resolve(handle);
			if (gpuBuf) 
				bufferBindings.push_back(SDL_GPUStorageBufferReadWriteBinding{ gpuBuf->GetNative(), false });
			
		}

		std::vector<SDL_GPUStorageTextureReadWriteBinding> textureBindings;
		textureBindings.reserve(storageTextures.size());
		for (const auto& handle : storageTextures) {
			auto* gpuTex = m_Cmd.GetGPUContext().GetTextureRegistry().Resolve(handle);
			if (gpuTex) {
				textureBindings.push_back(SDL_GPUStorageTextureReadWriteBinding{ gpuTex->GetNative(), 0, 0, false });
			}
		}

		m_Pass = SDL_BeginGPUComputePass(
			cmd.GetNative(),
			textureBindings.data(),
			static_cast<uint32_t>(textureBindings.size()),
			bufferBindings.data(),
			static_cast<uint32_t>(bufferBindings.size())
		);

		FALL_CORE_ASSERT(m_Pass, "Failed to begin GPU compute pass: {0}", SDL_GetError());
	}

	bool GPUComputePass::BindPipeline(PipelineHandle handle) {
		FALL_ASSERT_GPU_THREAD();

		auto* native = m_Cmd.GetGPUContext().GetComputePipelineRegistry().Resolve(handle);
		if (native) {
			SDL_BindGPUComputePipeline(m_Pass, native);
			return true;
		}

		return false;
	}

	void GPUComputePass::Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ) {
		FALL_ASSERT_GPU_THREAD();
		FALL_CORE_ASSERT(m_Pass, "Executing Dispatch on an invalid ComputePass");

		SDL_DispatchGPUCompute(m_Pass, groupsX, groupsY, groupsZ);
	}

	GPUComputePass::~GPUComputePass() {
		if (m_Pass) {
			SDL_EndGPUComputePass(m_Pass);
			m_Pass = nullptr;
		}
	}

}