#pragma once

#include <cstddef>
#include <functional>
#include "PipelineKey.h"

namespace FallEngine {

    struct PipelineKeyHash {
        size_t operator()(const PipelineKey& key) const {
            size_t h = 0;

            auto hashCombine = [&](size_t v) {
                h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
                };

            hashCombine(static_cast<size_t>(key.type));

            if (key.type == PipelineType::Compute) {
                hashCombine(reinterpret_cast<size_t>(key.computeShader));
                hashCombine(static_cast<size_t>(key.threadsX));
                hashCombine(static_cast<size_t>(key.threadsY));
                hashCombine(static_cast<size_t>(key.threadsZ));
            }
            else {
                hashCombine(reinterpret_cast<size_t>(key.vertexShader));
                hashCombine(reinterpret_cast<size_t>(key.fragmentShader));

                hashCombine(key.useBlending ? 1 : 0);
                hashCombine(static_cast<size_t>(key.targetFormat));
            }

            return h;
        }
    };
}