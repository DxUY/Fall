#pragma once

#include <SDL3/SDL_gpu.h>

struct SDL_Window;

namespace Fall {
    struct BackbufferView {
        void* opaque = nullptr;
        uint32_t width = 0;
        uint32_t height = 0;

        bool IsValid() const { return opaque != nullptr; }
    };

    class GPUContext {
    public:
        bool Initialize(void* window);
        void Shutdown();

        SDL_GPUCommandBuffer* AcquireCommandBuffer();
        void SubmitCommandBuffer(SDL_GPUCommandBuffer* cmd);

        BackbufferView AcquireBackbuffer(SDL_GPUCommandBuffer* cmd); 

        void SetVSync(bool enabled);

        SDL_GPUDevice* GetDevice() const { return m_Device; }
		SDL_GPUTextureFormat GetSwapchainFormat() const;

    private:
        SDL_Window* m_Window = nullptr;
        SDL_GPUDevice* m_Device = nullptr;
    };

}