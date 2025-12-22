#pragma once

#include "BufferView.h"
#include <filesystem>

namespace FallEngine {

    class MappedFile {
    public:
        MappedFile() = default;
        ~MappedFile();

        MappedFile(const MappedFile&) = delete;
        MappedFile& operator=(const MappedFile&) = delete;

        MappedFile(MappedFile&& other) noexcept;
        MappedFile& operator=(MappedFile&& other) noexcept;

        bool Open(const std::filesystem::path& path);
        void Close();

        bool IsOpen() const { return m_Data != nullptr; }
        size_t Size() const { return m_Size; }

        BufferView View() const { return BufferView(m_Data, m_Size); }

    private:
        void* m_Data = nullptr;
        size_t m_Size = 0;

#if defined(FALL_PLATFORM_WINDOWS)
        void* m_FileHandle = nullptr;
        void* m_MappingHandle = nullptr;
#else
#error "MappedFile only supports Windows in current engine configuration."
#endif
    };

}