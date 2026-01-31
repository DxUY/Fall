#pragma once
#include <cstddef>
#include "PipelineKey.h"

namespace Fall {

    struct PipelineKeyHash {
        size_t operator()(const PipelineKey& key) const {
            size_t h = 0;

            auto combine = [&](size_t v) {
                h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
                };

            combine(static_cast<size_t>(key.type));

            if (key.type == PipelineType::Compute) {
                combine(reinterpret_cast<size_t>(key.computeShader));
                combine(key.threadsX);
                combine(key.threadsY);
                combine(key.threadsZ);
            }
            else {
                combine(reinterpret_cast<size_t>(key.vertexShader));
                combine(reinterpret_cast<size_t>(key.fragmentShader));
                combine(key.useBlending ? 1u : 0u);
                combine(key.targetFormat);
            }

            return h;
        }
    };

}