#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace FallEngine {

    class ComputeShader;
    class GraphicsShader;

    // --------------------------------------------------
    // Render pass compatibility signature
    // --------------------------------------------------
    struct RenderPassSignature {
        std::vector<SDL_GPUTextureFormat> colorFormats;
        SDL_GPUTextureFormat depthFormat = SDL_GPU_TEXTUREFORMAT_INVALID;
    };

    inline bool operator==(const RenderPassSignature& a,
        const RenderPassSignature& b) {
        return a.colorFormats == b.colorFormats &&
            a.depthFormat == b.depthFormat;
    }

    struct RenderPassSignatureHash {
        size_t operator()(const RenderPassSignature& s) const noexcept;
    };

    // --------------------------------------------------
    // Vertex layout signature (SDL-native)
    // --------------------------------------------------
    struct VertexLayoutSignature {
        std::vector<SDL_GPUVertexAttribute> attributes;
        std::vector<SDL_GPUVertexBufferDescription> buffers;
    };

    inline bool operator==(const VertexLayoutSignature& a,
        const VertexLayoutSignature& b) {
        if (a.attributes.size() != b.attributes.size() ||
            a.buffers.size() != b.buffers.size())
            return false;

        for (size_t i = 0; i < a.attributes.size(); ++i) {
            const auto& x = a.attributes[i];
            const auto& y = b.attributes[i];
            if (x.location != y.location ||
                x.format != y.format ||
                x.offset != y.offset ||
                x.buffer_slot != y.buffer_slot)
                return false;
        }

        for (size_t i = 0; i < a.buffers.size(); ++i) {
            const auto& x = a.buffers[i];
            const auto& y = b.buffers[i];
            if (x.slot != y.slot ||
                x.pitch != y.pitch ||
                x.input_rate != y.input_rate)
                return false;
        }

        return true;
    }

    struct VertexLayoutSignatureHash {
        size_t operator()(const VertexLayoutSignature& v) const noexcept;
    };

    // --------------------------------------------------
    // Pipeline system
    // --------------------------------------------------
    class Pipeline {
    public:
        static void Init(SDL_GPUDevice* device);
        static void Shutdown();

        static SDL_GPUComputePipeline* GetComputePipeline(
            const ComputeShader& shader);

        static SDL_GPUGraphicsPipeline* GetGraphicsPipeline(
            const GraphicsShader& shader,
            const VertexLayoutSignature& layout,
            const RenderPassSignature& pass);

    private:
        inline static SDL_GPUDevice* s_Device = nullptr;
    };

}