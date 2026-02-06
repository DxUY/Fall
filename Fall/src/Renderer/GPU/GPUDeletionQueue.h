#pragma once

#include <vector>
#include <functional>

namespace Fall {

    class GPUDeletionQueue {
    public:
        void Push(std::function<void()>&& deallocator) {
            m_Deletors.emplace_back(std::move(deallocator));
        }

        void Flush() {
            for (auto it = m_Deletors.rbegin(); it != m_Deletors.rend(); ++it) {
                (*it)();
            }
            m_Deletors.clear();
        }

    private:
        std::vector<std::function<void()>> m_Deletors;
    };

}