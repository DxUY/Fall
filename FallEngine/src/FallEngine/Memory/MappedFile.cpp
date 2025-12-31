#include "FallEnginePCH.h"
#include "MappedFile.h"

namespace FallEngine {

    MappedFile::~MappedFile() {
        Close();
    }

    MappedFile::MappedFile(MappedFile&& other) noexcept
        : m_Data(other.m_Data),
        m_Size(other.m_Size)
    {
#if defined(FALL_PLATFORM_WINDOWS)
        m_FileHandle = other.m_FileHandle;
        m_MappingHandle = other.m_MappingHandle;
        other.m_FileHandle = (void*)-1;
        other.m_MappingHandle = nullptr;
#endif
        other.m_Data = nullptr;
        other.m_Size = 0;
    }

    MappedFile& MappedFile::operator=(MappedFile&& other) noexcept {
        if (this != &other) {
            Close();

            m_Data = other.m_Data;
            m_Size = other.m_Size;
#if defined(FALL_PLATFORM_WINDOWS)
            m_FileHandle = other.m_FileHandle;
            m_MappingHandle = other.m_MappingHandle;
            other.m_FileHandle = (void*)-1;
            other.m_MappingHandle = nullptr;
#endif
            other.m_Data = nullptr;
            other.m_Size = 0;
        }
        return *this;
    }

    void MappedFile::Close() {
#if defined(FALL_PLATFORM_WINDOWS)
        if (m_Data) {
            UnmapViewOfFile(m_Data);
            m_Data = nullptr;
        }

        if (m_MappingHandle) {
            CloseHandle(m_MappingHandle);
            m_MappingHandle = nullptr;
        }

        if (m_FileHandle != (void*)-1) {
            CloseHandle(m_FileHandle);
            m_FileHandle = (void*)-1;
        }
#endif
        m_Size = 0;
    }

    bool MappedFile::Open(const std::string& path) {
        Close();

#if defined(FALL_PLATFORM_WINDOWS)
        m_FileHandle = CreateFileA(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (m_FileHandle == (void*)-1) {
            return false;
        }

        LARGE_INTEGER size;
        if (!GetFileSizeEx(m_FileHandle, &size) || size.QuadPart == 0) {
            Close();
            return false;
        }

        m_Size = static_cast<size_t>(size.QuadPart);

        m_MappingHandle = CreateFileMappingA(
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
#else
        return false;
#endif
    }

}