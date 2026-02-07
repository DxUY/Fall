#pragma once

#include "GPUDeletionQueue.h"

#include <functional>

struct SDL_Window;

struct SDL_GPUDevice;
struct SDL_GPUCommandBuffer;
struct SDL_GPUFence;

enum SDL_GPUTextureFormat : int;

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

        void EnqueueDeletion(std::function<void()>&& deallocator);
        void SyncCleanup(SDL_GPUFence* fence);

        SDL_GPUDevice* GetDevice() const { return m_Device; }
		SDL_GPUTextureFormat GetSwapchainFormat() const;

    private:
        SDL_Window* m_Window = nullptr;
        SDL_GPUDevice* m_Device = nullptr;
		GPUDeletionQueue m_DeletionQueue;
    };

}