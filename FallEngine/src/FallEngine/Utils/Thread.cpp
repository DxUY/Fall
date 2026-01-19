#include "FallEngine/Core/Base.h"
#include <thread>

namespace FallEngine
{
    static std::thread::id s_GPUThreadID;
    static bool s_GPUThreadIDInitialized = false;

    void SetGPUThreadID(bool force)
    {
        if (!s_GPUThreadIDInitialized || force)
        {
            s_GPUThreadID = std::this_thread::get_id();
            s_GPUThreadIDInitialized = true;
        }
    }

    bool IsOnGPUThread()
    {
        FALL_CORE_ASSERT(s_GPUThreadIDInitialized, "GPU thread not initialized");
        return std::this_thread::get_id() == s_GPUThreadID;
    }
}