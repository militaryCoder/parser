#include "file.hpp"

#include <stx/panic.h>

#include <cstdint>
#include <iostream>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    using WndErrorCode = DWORD;
#else
    #include <stdio.h>
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
            const bool readSuccessful = ReadFile(fileHandle, (void *)buf_, fileSize, &bytesRead, nullptr);
            if (!readSuccessful) {
                std::cerr << "Could not read \"" << name_ << "\"\n";
                std::cerr << "Reason: ";
                const WndErrorCode err = GetLastError();
                // TODO: should replace std::cerr's with some logger stuff
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

    size_t getFileSize(FILE *f) {
        const long offset = 0l;
        fseek(f, offset, SEEK_END);
        const size_t size = ftell(f);
        rewind(f);

        return size;
    }

    File::File(const char *filename, Mode &&m)
        : name_(filename) {
        const char *accessMode = Mode::Read == std::move(m) ? "r" : "w";
        FILE *file = fopen(name_.c_str(), accessMode);
        if (file) {
            const size_t fsize = getFileSize(file);
            buf_ = new char[fsize];
            const size_t readCount = fread((void *)buf_, sizeof(char), fsize, file);
            if (!(fsize == readCount)) {
                std::cerr << "File was read partially.\n";
            }
        }
        else {
            std::cerr << "An error occured while reading \"" << name_ << "\".";
        }
    }
    File::File(const Str &filename, Mode &&m)
        : File(filename.c_str(), std::move(m)) {}
    
#endif

    char File::readChar() {
        return buf_[caret_++];
    }

    Str File::readLine() {
        std::stringstream ss;
        do {
            ss << buf_[caret_++];
        } while ('\n' != buf_[caret_]);
        caret_++;

        return ss.str();
    }

    char File::peekChar() {
        return buf_[caret_ + 1];
    }

    File::~File() {
        if (buf_)
            delete[] buf_;
    }
}
