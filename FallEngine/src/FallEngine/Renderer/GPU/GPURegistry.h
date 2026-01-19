#pragma once

#include <unordered_map>
#include <mutex>

namespace FallEngine {

    template<typename THandle, typename TNative>
    class GPURegistry {
    public:
        void Register(THandle handle, TNative* native) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Entries[handle.id] = native;
        }

        bool Exists(THandle handle) const {
            std::lock_guard<std::mutex> lock(m_Mutex);
            return m_Entries.find(handle.id) != m_Entries.end();
        }

        TNative* Resolve(THandle handle) const {
            std::lock_guard<std::mutex> lock(m_Mutex);
            auto it = m_Entries.find(handle.id);
            return (it != m_Entries.end()) ? it->second : nullptr;
        }

        void Remove(THandle handle) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Entries.erase(handle.id);
        }

        std::unordered_map<uint32_t, TNative*> ExtractAll() {
            std::lock_guard<std::mutex> lock(m_Mutex);
            std::unordered_map<uint32_t, TNative*> ret = std::move(m_Entries);
            m_Entries.clear();
            return ret;
        }

    private:
        std::unordered_map<uint32_t, TNative*> m_Entries;
        mutable std::mutex m_Mutex; 
    };

}