#pragma once
#include <cstdint>
#include <cstring>

namespace FallEngine {

	class Buffer {
	public:
		Buffer() = default;
		explicit Buffer(size_t size)
			: m_Size(size), m_Data(new uint8_t[size]) {
		}

		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		Buffer(Buffer&& other) noexcept
			: m_Data(other.m_Data), m_Size(other.m_Size) {
			other.m_Data = nullptr;
			other.m_Size = 0;
		}

		Buffer& operator=(Buffer&& other) noexcept {
			if (this != &other) {
				delete[] m_Data;
				m_Data = other.m_Data;
				m_Size = other.m_Size;

				other.m_Data = nullptr;
				other.m_Size = 0;
			}
			return *this;
		}

		~Buffer() { delete[] m_Data; }

		inline uint8_t* Data() { return m_Data; }
		inline const uint8_t* Data() const { return m_Data; }
		inline size_t Size() const { return m_Size; }

		template<typename T>
		inline T* As() { return reinterpret_cast<T*>(m_Data); }

	private:
		uint8_t* m_Data = nullptr;
		size_t m_Size = 0;
	};

}