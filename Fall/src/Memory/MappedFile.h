#pragma once

#include "BufferView.h"
#include <string>

namespace Fall {

    class MappedFile {
    public:
        MappedFile() = default;
        ~MappedFile();

        MappedFile(const MappedFile&) = delete;
        MappedFile& operator=(const MappedFile&) = delete;

        MappedFile(MappedFile&& other) noexcept;
        MappedFile& operator=(MappedFile&& other) noexcept;

        bool Open(const std::string& path);
        void Close();

        bool IsOpen() const { return m_Data != nullptr; }
        size_t Size() const { return m_Size; }

        BufferView View() const { return BufferView(m_Data, m_Size); }

    private:
        void* m_Data = nullptr;
        size_t m_Size = 0;

#if defined(FALL_PLATFORM_WINDOWS)
    #include <Windows.h>
        HANDLE m_FileHandle = INVALID_HANDLE_VALUE;
        HANDLE m_MappingHandle = nullptr;
#endif
    };

}