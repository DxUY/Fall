#pragma once

#include "PipelineKey.h"
#include "PipelineHandle.h"

namespace FallEngine {
    struct PipelineJob {
        PipelineKey key;
        PipelineHandle handle;
    };
}