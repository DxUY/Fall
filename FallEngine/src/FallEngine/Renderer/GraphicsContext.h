#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

struct SDL_Window;

namespace FallEngine {

    class GraphicsContext {
    public:
        GraphicsContext(SDL_Window* windowHandle);
        ~GraphicsContext() = default;

        void Init();
        void SwapBuffers();
    private:
        SDL_Window* m_WindowHandle = nullptr;
        SDL_GPUDevice* m_GPUDevice = nullptr;
		SDL_GPUGraphicsPipeline* m_Pipeline = nullptr;
    };
}
