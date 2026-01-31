#pragma once

#include <cstdint>
#include <vector>

namespace Fall {

    class Buffer {
    public:
        Buffer() = default;
        explicit Buffer(size_t size) : m_Data(size) {}

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        Buffer(Buffer&& other) noexcept = default;
        Buffer& operator=(Buffer&& other) noexcept = default;

        ~Buffer() = default;

        inline uint8_t* Data() { return m_Data.empty() ? nullptr : m_Data.data(); }
        inline const uint8_t* Data() const { return m_Data.empty() ? nullptr : m_Data.data(); }
        inline size_t Size() const { return m_Data.size(); }

        template<typename T>
        inline T* As() { return reinterpret_cast<T*>(Data()); }

    private:
        std::vector<uint8_t> m_Data;
    };

}