#pragma once

#include "FallEngine/Core/Base.h"

#include "Renderer/Resource/Texture.h"
#include "Renderer/Resource/Buffer.h"
#include "Renderer/Pipeline/PipelineHandle.h" 

#include "GPURegistry.h"

struct SDL_Window;
struct SDL_GPUDevice;
struct SDL_GPUCommandBuffer;
struct SDL_GPUGraphicsPipeline;
struct SDL_GPUComputePipeline;

namespace FallEngine {

    class GPUBuffer;
    class GPUTexture;

    struct BackbufferView {
        void* opaque = nullptr;
        uint32_t width = 0;
        uint32_t height = 0;

        bool IsValid() const { return opaque != nullptr; }
    };

    class GPUContext {
    public:
        GPUContext() = default;
        ~GPUContext();

        FALL_NON_COPYABLE(GPUContext)

            bool Initialize(void* window);
        void Shutdown();

        BackbufferView AcquireBackbuffer(SDL_GPUCommandBuffer* cmd);

        SDL_GPUCommandBuffer* AcquireCommandBuffer();
        void SubmitCommandBuffer(SDL_GPUCommandBuffer* cmd);

        GPURegistry<Texture, GPUTexture>& GetTextureRegistry() { return m_TextureRegistry; }
        GPURegistry<Buffer, GPUBuffer>& GetBufferRegistry() { return m_BufferRegistry; }

        GPURegistry<PipelineHandle, SDL_GPUGraphicsPipeline>& GetGraphicsPipelineRegistry() { return m_GraphicsPipelines; }
        GPURegistry<PipelineHandle, SDL_GPUComputePipeline>& GetComputePipelineRegistry() { return m_ComputePipelines; }

        SDL_GPUDevice* GetDevice() const { return m_Device; }
        SDL_Window* GetWindow() const { return m_Window; }

    private:
        SDL_Window* m_Window = nullptr;
        SDL_GPUDevice* m_Device = nullptr;

        GPURegistry<Texture, GPUTexture> m_TextureRegistry;
        GPURegistry<Buffer, GPUBuffer> m_BufferRegistry;

        GPURegistry<PipelineHandle, SDL_GPUGraphicsPipeline> m_GraphicsPipelines;
        GPURegistry<PipelineHandle, SDL_GPUComputePipeline> m_ComputePipelines;
    };

}