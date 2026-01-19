#pragma once

#include "FallEngine/Core/Base.h"
#include "Renderer/Pipeline/PipelineHandle.h"
#include "Renderer/Resource/Buffer.h"
#include "Renderer/Resource/Texture.h"

#include <vector>

struct SDL_GPUComputePass;

namespace FallEngine {

	class GPUCommand;

	class GPUComputePass {
	public:
		GPUComputePass(
			GPUCommand& cmd,
			const std::vector<Buffer>& storageBuffers = {},
			const std::vector<Texture>& storageTextures = {}
		);
		~GPUComputePass();

		FALL_NON_COPYABLE(GPUComputePass)

		bool BindPipeline(PipelineHandle handle);
		void Dispatch(uint32_t groupsX, uint32_t groupsY, uint32_t groupsZ);

	private:
		GPUCommand& m_Cmd;
		SDL_GPUComputePass* m_Pass = nullptr;
	};

}