target_sources(Fall PRIVATE
	src/Renderer/Camera/OrthographicCamera.cpp
)

target_sources(Fall PRIVATE
	src/Renderer/Core/FrameContext.cpp
	src/Renderer/Core/Renderer.cpp
)

target_sources(Fall PRIVATE
	src/Renderer/GPU/GPUBuffer.cpp
	src/Renderer/GPU/GPUCommand.cpp
	src/Renderer/GPU/GPUComputePass.cpp
	src/Renderer/GPU/GPUComputePipeline.cpp
	src/Renderer/GPU/GPUContext.cpp
	src/Renderer/GPU/GPUGraphicsPipeline.cpp
	src/Renderer/GPU/GPURenderPass.cpp
	src/Renderer/GPU/GPURenderTarget.cpp
	src/Renderer/GPU/GPURingBuffer.cpp
	src/Renderer/GPU/GPUShader.cpp
	src/Renderer/GPU/GPUTexture.cpp
	src/Renderer/GPU/GPUUtils.cpp
)

target_sources(Fall PRIVATE
	src/Renderer/Pass/Implementations/ClearPass.cpp
	src/Renderer/Pass/Implementations/ForwardRenderPass.cpp
	src/Renderer/Pass/Implementations/ImGuiPass.cpp
)

target_sources(Fall PRIVATE
	src/Renderer/Resource/Pipeline/PipelineManager.cpp
)