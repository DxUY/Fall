#pragma once

#include <SDL3/SDL_gpu.h>

struct SDL_Window;

namespace FallEngine {

    class GraphicsContext {
    public:
        GraphicsContext(SDL_Window* windowHandle);
        ~GraphicsContext();

        void Init();

        SDL_GPUDevice* GetDevice() const { return m_GPUDevice; }

    private:
        SDL_Window* m_WindowHandle;
        SDL_GPUDevice* m_GPUDevice;
    };

}
