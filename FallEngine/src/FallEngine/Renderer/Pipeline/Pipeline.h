#pragma once

#include "PipelineHandle.h"
#include "PipelineKey.h"
#include "PipelineJob.h"
#include "PipelineKeyHash.h"

#include <unordered_map>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <string>

namespace FallEngine {

    class GPUContext;
    class GPUPipeline;

    class Pipeline {
    public:
        Pipeline(GPUContext& gpu);
        ~Pipeline();

        FALL_NON_COPYABLE(Pipeline)

        PipelineHandle RequestPipeline(const PipelineKey& key);

        PipelineHandle GetActiveHandle(PipelineHandle handle) const;

        void ProcessCompletedJobs();

    private:
        void WorkerLoop();
        void CreateFallbackPipeline();

    private:
        GPUContext& m_GPU;
        Scope<GPUPipeline> m_Internal;

        PipelineHandle m_FallbackHandle;
        uint32_t m_NextID = 1;

        std::unordered_map<PipelineKey, PipelineHandle, PipelineKeyHash> m_Cache;
        PipelineKeyHash m_Hasher;

        std::mutex m_JobMutex;
        std::condition_variable m_JobCV;
        std::vector<PipelineJob> m_JobQueue;

        std::mutex m_CompletedMutex;
        std::vector<PipelineJob> m_CompletedQueue;

        std::thread m_WorkerThread;
        std::atomic<bool> m_Running{ true };
    };

}