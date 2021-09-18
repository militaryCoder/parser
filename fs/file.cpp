#include "file.hpp"

#include <stx/panic.h>

#include <cstdint>
#include <iostream>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    using WndErrorCode = DWORD;
#endif

namespace fs
{
#if defined(_WIN32) || defined(_WIN64)

    DWORD convertAccessModeToWndFlag(Mode m)
    {
        switch (static_cast<int>(m))
        {
            case static_cast<int>(Mode::Read):
                return GENERIC_READ;
            case static_cast<int>(Mode::Write):
                return GENERIC_WRITE;
            default:
                return GENERIC_READ | GENERIC_WRITE;
        }
    }

    long long getWndFileSize(HANDLE fileHandle)
    {
        LARGE_INTEGER fsz;
        GetFileSizeEx(fileHandle, &fsz);

        return fsz.QuadPart;
    }

    File::File(const char *filename, Mode &&m)
        : name_(filename) {
        const DWORD NON_SHARABLE = 0;
        const DWORD accessMode = convertAccessModeToWndFlag(std::move(m));
        HANDLE fileHandle = CreateFile(filename,
                                       accessMode,
                                       NON_SHARABLE,
                                       nullptr,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       nullptr);
        
        if (nullptr != fileHandle) {
            const long long fileSize = getWndFileSize(fileHandle);
            DWORD bytesRead;
            buf_ = new char[fileSize];
            length_ = fileSize;
            const bool readSuccessful = ReadFile(fileHandle, (void *)buf_, fileSize, &bytesRead, nullptr);
            if (!readSuccessful) {
                std::cerr << "Could not read \"" << name_ << "\"\n";
                std::cerr << "Reason: ";
                const WndErrorCode err = GetLastError();
                // TODO: mayber should replace std::cerr's with some logger stuff
                switch(err) {
                    case ERROR_INSUFFICIENT_BUFFER:
                        std::cerr << "insufficient buffer.\n";
                        break;
                    case ERROR_INVALID_USER_BUFFER:
                        std::cerr << "invalid user buffer.\n"; 
                        break;
                    case ERROR_NOT_ENOUGH_MEMORY:
                        std::cerr << "not enough memory.\n";
                        break;
                    case ERROR_BROKEN_PIPE:
                        std::cerr << "broken pipe.\n";
                        break;
                    default:
                        std::cerr << "unknown reason.\n";
                }
            }
        }
    }
    File::File(const Str &filename, Mode &&m)
        : File(filename.c_str(), std::move(m)) {}

#else

    // TODO: should be reading into buffer
    File::File(const char *filename, Mode m)
        : name_(filename) {
        file_ = fopen(name_, static_cast<const char *>(m));
    }
    
    bool File::isValid() const {
        return nullptr != file_;
    }

#endif

    char File::readChar() {
        return buf_[caret_++];
    }

    Str File::readLine() {
        std::stringstream ss;
        do {
            ss << buf_[caret_++];
        } while ('\n' != buf_[caret_]);

        return ss.str();
    }

    File::~File() {
        if (buf_)
            delete[] buf_;
    }
}
