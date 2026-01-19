#include "FallEnginePCH.h"
#include "Pipeline.h"

#include "Renderer/GPU/GPUPipeline.h"
#include "Renderer/GPU/GPUContext.h"

namespace FallEngine {

    Pipeline::Pipeline(GPUContext& gpu) : m_GPU(gpu) {
        m_Internal = CreateScope<GPUPipeline>(gpu);

        CreateFallbackPipeline();

        m_WorkerThread = std::thread(&Pipeline::WorkerLoop, this);
    }

    Pipeline::~Pipeline() {
        m_Running = false;

        m_JobCV.notify_all();

        if (m_WorkerThread.joinable()) {
            m_WorkerThread.join();
        }

        FALL_CORE_INFO("Pipeline Manager shut down cleanly - Worker Thread stopped.");
    }

    PipelineHandle Pipeline::RequestPipeline(const PipelineKey& key) {
        std::lock_guard<std::mutex> lock(m_JobMutex);

        auto it = m_Cache.find(key);
        if (it != m_Cache.end()) return it->second;

        PipelineHandle handle{ m_NextID++ };
        m_Cache[key] = handle;

        m_JobQueue.push_back({ key, handle });
        m_JobCV.notify_one();

        return handle;
    }

    void Pipeline::WorkerLoop() {
        while (m_Running) {
            PipelineJob job;
            {
                std::unique_lock<std::mutex> lock(m_JobMutex);
                m_JobCV.wait(lock, [this] { return !m_JobQueue.empty() || !m_Running; });

                if (!m_Running) break;

                job = std::move(m_JobQueue.back());
                m_JobQueue.pop_back();
            }

            {
                std::lock_guard<std::mutex> lock(m_CompletedMutex);
                m_CompletedQueue.push_back(job);
            }
        }
    }

    void Pipeline::ProcessCompletedJobs() {
        FALL_ASSERT_GPU_THREAD();

        std::vector<PipelineJob> readyJobs;
        {
            std::lock_guard<std::mutex> lock(m_CompletedMutex);
            if (m_CompletedQueue.empty()) return;
            readyJobs = std::move(m_CompletedQueue);
            m_CompletedQueue.clear();
        }

        for (const auto& job : readyJobs) {
            m_Internal->CreateAndRegister(job.handle, job.key);
        }
    }

    PipelineHandle Pipeline::GetActiveHandle(PipelineHandle handle) const {
        if (m_GPU.GetGraphicsPipelineRegistry().Exists(handle)) {
            return handle;
        }
        return m_FallbackHandle;
    }

    void Pipeline::CreateFallbackPipeline() {
        PipelineKey fallbackKey;
        m_FallbackHandle = { m_NextID++ };
        m_Internal->CreateAndRegister(m_FallbackHandle, fallbackKey);
    }
}