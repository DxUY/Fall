#include "Pipeline.h"
#include "Shaders/GraphicsShader.h"
#include "Shaders/ComputeShader.h"

#include <cassert>
#include <functional>

namespace FallEngine {

    // --------------------------------------------------
    // Hash helper
    // --------------------------------------------------
    template<typename T>
    static inline void HashCombine(size_t& seed, const T& v) {
        seed ^= std::hash<T>{}(v)
            +0x9e3779b9
            + (seed << 6)
            + (seed >> 2);
    }

    // --------------------------------------------------
    // Hash implementations
    // --------------------------------------------------
    size_t RenderPassSignatureHash::operator()(
        const RenderPassSignature& s) const noexcept {

        size_t h = 0;
        for (auto f : s.colorFormats)
            HashCombine(h, static_cast<uint32_t>(f));

        HashCombine(h, static_cast<uint32_t>(s.depthFormat));
        return h;
    }

    size_t VertexLayoutSignatureHash::operator()(
        const VertexLayoutSignature& v) const noexcept {

        size_t h = 0;

        for (const auto& a : v.attributes) {
            HashCombine(h, a.location);
            HashCombine(h, static_cast<uint32_t>(a.format));
            HashCombine(h, a.offset);
            HashCombine(h, a.buffer_slot);
        }

        for (const auto& b : v.buffers) {
            HashCombine(h, b.slot);
            HashCombine(h, b.pitch);
            HashCombine(h, static_cast<uint32_t>(b.input_rate));
        }

        return h;
    }

    // --------------------------------------------------
    // Internal pipeline keys
    // --------------------------------------------------
    struct GraphicsPipelineKey {
        const GraphicsShader* shader;
        VertexLayoutSignature layout;
        RenderPassSignature pass;

        bool operator==(const GraphicsPipelineKey& o) const {
            return shader == o.shader &&
                layout == o.layout &&
                pass == o.pass;
        }
    };

    struct GraphicsPipelineKeyHash {
        size_t operator()(const GraphicsPipelineKey& k) const noexcept {
            size_t h = std::hash<const void*>{}(k.shader);
            HashCombine(h, VertexLayoutSignatureHash{}(k.layout));
            HashCombine(h, RenderPassSignatureHash{}(k.pass));
            return h;
        }
    };

    struct ComputePipelineKey {
        const ComputeShader* shader;

        bool operator==(const ComputePipelineKey& o) const {
            return shader == o.shader;
        }
    };

    struct ComputePipelineKeyHash {
        size_t operator()(const ComputePipelineKey& k) const noexcept {
            return std::hash<const void*>{}(k.shader);
        }
    };

    // --------------------------------------------------
    // Caches
    // --------------------------------------------------
    static std::unordered_map<
        GraphicsPipelineKey,
        SDL_GPUGraphicsPipeline*,
        GraphicsPipelineKeyHash
    > s_GraphicsPipelines;

    static std::unordered_map<
        ComputePipelineKey,
        SDL_GPUComputePipeline*,
        ComputePipelineKeyHash
    > s_ComputePipelines;

    // --------------------------------------------------
    // Lifecycle
    // --------------------------------------------------
    void Pipeline::Init(SDL_GPUDevice* device) {
        assert(device);
        s_Device = device;
    }

    void Pipeline::Shutdown() {
        for (auto& [_, p] : s_GraphicsPipelines)
            SDL_ReleaseGPUGraphicsPipeline(s_Device, p);

        for (auto& [_, p] : s_ComputePipelines)
            SDL_ReleaseGPUComputePipeline(s_Device, p);

        s_GraphicsPipelines.clear();
        s_ComputePipelines.clear();
        s_Device = nullptr;
    }

    // --------------------------------------------------
    // Compute pipeline
    // --------------------------------------------------
    SDL_GPUComputePipeline* Pipeline::GetComputePipeline(
        const ComputeShader& shader) {

        assert(s_Device);

        ComputePipelineKey key{ &shader };
        auto it = s_ComputePipelines.find(key);
        if (it != s_ComputePipelines.end())
            return it->second;

        const ShaderBlob& blob = shader.GetBlob();

        SDL_GPUComputePipelineCreateInfo info{};
        info.code = blob.code;
        info.code_size = blob.size;
        info.entrypoint = blob.entrypoint;
        info.format = blob.format;
        info.threadcount_x = shader.ThreadX();
        info.threadcount_y = shader.ThreadY();
        info.threadcount_z = shader.ThreadZ();

        SDL_GPUComputePipeline* pipeline =
            SDL_CreateGPUComputePipeline(s_Device, &info);

        assert(pipeline);
        s_ComputePipelines.emplace(key, pipeline);
        return pipeline;
    }

    // --------------------------------------------------
    // Graphics pipeline
    // --------------------------------------------------
    SDL_GPUGraphicsPipeline* Pipeline::GetGraphicsPipeline(
        const GraphicsShader& shader,
        const VertexLayoutSignature& layout,
        const RenderPassSignature& pass) {

        assert(s_Device);
        assert(!pass.colorFormats.empty());

        GraphicsPipelineKey key{ &shader, layout, pass };
        auto it = s_GraphicsPipelines.find(key);
        if (it != s_GraphicsPipelines.end())
            return it->second;

        // ---- Vertex input (use OWNED key memory)
        SDL_GPUVertexInputState vertexInput{};
        vertexInput.vertex_attributes =
            key.layout.attributes.data();
        vertexInput.num_vertex_attributes =
            static_cast<Uint32>(key.layout.attributes.size());
        vertexInput.vertex_buffer_descriptions =
            key.layout.buffers.data();
        vertexInput.num_vertex_buffers =
            static_cast<Uint32>(key.layout.buffers.size());

        // ---- Render targets (temporary, consumed immediately)
        std::vector<SDL_GPUColorTargetDescription> colorTargets;
        colorTargets.reserve(key.pass.colorFormats.size());

        for (auto format : key.pass.colorFormats) {
            SDL_GPUColorTargetDescription desc{};
            desc.format = format;
            colorTargets.push_back(desc);
        }

        SDL_GPUGraphicsPipelineTargetInfo targetInfo{};
        targetInfo.color_target_descriptions = colorTargets.data();
        targetInfo.num_color_targets =
            static_cast<Uint32>(colorTargets.size());
        targetInfo.depth_stencil_format = key.pass.depthFormat;
        targetInfo.has_depth_stencil_target =
            key.pass.depthFormat != SDL_GPU_TEXTUREFORMAT_INVALID;

        SDL_GPUGraphicsPipelineCreateInfo info{};
        info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
        info.vertex_shader = shader.GetVertexShader();
        info.fragment_shader = shader.GetFragmentShader();
        info.vertex_input_state = vertexInput;
        info.target_info = targetInfo;

        SDL_GPUGraphicsPipeline* pipeline =
            SDL_CreateGPUGraphicsPipeline(s_Device, &info);

        assert(pipeline);
        s_GraphicsPipelines.emplace(key, pipeline);
        return pipeline;
    }

}