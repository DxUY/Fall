#pragma once

#include <cstdint>

namespace FallEngine {

    struct PipelineHandle {
        uint32_t id = 0;

        bool IsValid() const { return id != 0; }

        bool operator==(const PipelineHandle& other) const {
            return id == other.id;
        }

        bool operator!=(const PipelineHandle& other) const {
            return id != other.id;
        }
    };

}