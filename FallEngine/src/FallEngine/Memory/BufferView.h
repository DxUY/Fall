#pragma once
#include <cstdint>
#include <cstddef>

namespace FallEngine {

	class BufferView {
	public:
		BufferView() = default;
		BufferView(void* data, size_t size)
			: m_Data(reinterpret_cast<uint8_t*>(data)), m_Size(size) {
		}

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