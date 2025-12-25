#include "FallEnginePCH.h"
#include "MappedFile.h"

#include <filesystem>

namespace FallEngine {

    MappedFile::~MappedFile() {
        Close();
    }

    MappedFile::MappedFile(MappedFile&& other) noexcept
        : m_Data(other.m_Data),
        m_Size(other.m_Size),
        m_FileHandle(other.m_FileHandle),
        m_MappingHandle(other.m_MappingHandle)
    {
        other.m_Data = nullptr;
        other.m_Size = 0;
        other.m_FileHandle = nullptr;
        other.m_MappingHandle = nullptr;
    }

    MappedFile& MappedFile::operator=(MappedFile&& other) noexcept {
        if (this != &other) {
            Close();

            m_Data = other.m_Data;
            m_Size = other.m_Size;
            m_FileHandle = other.m_FileHandle;
            m_MappingHandle = other.m_MappingHandle;

            other.m_Data = nullptr;
            other.m_Size = 0;
            other.m_FileHandle = nullptr;
            other.m_MappingHandle = nullptr;
        }
        return *this;
    }

    void MappedFile::Close() {
        if (!m_Data)
            return;

        UnmapViewOfFile(m_Data);
        m_Data = nullptr;

        if (m_MappingHandle) {
            CloseHandle(m_MappingHandle);
            m_MappingHandle = nullptr;
        }

        if (m_FileHandle) {
            CloseHandle(m_FileHandle);
            m_FileHandle = nullptr;
        }

        m_Size = 0;
    }

    bool MappedFile::Open(const std::string& path) {
        Close(); 

        m_FileHandle = CreateFileA(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (m_FileHandle == INVALID_HANDLE_VALUE)
            return false;

        LARGE_INTEGER size;
        if (!GetFileSizeEx(m_FileHandle, &size)) {
            Close();
            return false;
        }

        m_Size = static_cast<size_t>(size.QuadPart);

        m_MappingHandle = CreateFileMappingW(
            m_FileHandle,
            nullptr,
            PAGE_READONLY,
            0, 0,
            nullptr
        );

        if (!m_MappingHandle) {
            Close();
            return false;
        }

        m_Data = MapViewOfFile(
            m_MappingHandle,
            FILE_MAP_READ,
            0, 0,
            0
        );

        if (!m_Data) {
            Close();
            return false;
        }

        return true;
    }

}
