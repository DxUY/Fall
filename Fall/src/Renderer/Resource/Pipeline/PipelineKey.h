#pragma once

#include <cstdint>
#include <tuple> 

namespace Fall {

    enum class PipelineType : uint8_t { Graphics, Compute };

    class GraphicsShader;
    class ComputeShader;

    struct PipelineKey {
        PipelineType type = PipelineType::Graphics;

        const GraphicsShader* vertexShader = nullptr;
        const GraphicsShader* fragmentShader = nullptr;

        const ComputeShader* computeShader = nullptr;

        bool useBlending = true;
        uint32_t targetFormat = 0;

        uint32_t threadsX = 8;
        uint32_t threadsY = 8;
        uint32_t threadsZ = 1;

        bool operator==(const PipelineKey& rhs) const {
            if (type != rhs.type) return false;

            if (type == PipelineType::Compute) {
                return computeShader == rhs.computeShader &&
                    threadsX == rhs.threadsX &&
                    threadsY == rhs.threadsY &&
                    threadsZ == rhs.threadsZ;
            }

            return vertexShader == rhs.vertexShader &&
                fragmentShader == rhs.fragmentShader &&
                useBlending == rhs.useBlending &&
                targetFormat == rhs.targetFormat;
        }

        bool operator<(const PipelineKey& rhs) const {
            if (type != rhs.type) return type < rhs.type;

            if (type == PipelineType::Compute) {
                return std::tie(computeShader, threadsX, threadsY, threadsZ) <
                    std::tie(rhs.computeShader, rhs.threadsX, rhs.threadsY, rhs.threadsZ);
            }

            return std::tie(vertexShader, fragmentShader, useBlending, targetFormat) <
                std::tie(rhs.vertexShader, rhs.fragmentShader, rhs.useBlending, rhs.targetFormat);
        }
    };

}