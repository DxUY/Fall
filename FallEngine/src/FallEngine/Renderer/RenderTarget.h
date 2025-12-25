#pragma once

namespace FallEngine {

    class Texture; 

    struct RenderTarget {
        Texture* Color = nullptr;
        Texture* Depth = nullptr;

        bool IsSwapchain = false;

        static RenderTarget Swapchain()
        {
            RenderTarget rt{};
            rt.IsSwapchain = true;
            return rt;
        }

        static RenderTarget Offscreen(Texture* color, Texture* depth = nullptr)
        {
            RenderTarget rt{};
            rt.Color = color;
            rt.Depth = depth;
            rt.IsSwapchain = false;
            return rt;
        }

        bool IsValid() const
        {
            return IsSwapchain || Color != nullptr;
        }
    };

}