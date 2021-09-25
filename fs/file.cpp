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

typedef long long llong;

namespace fs {
#if defined(_WIN32) || defined(_WIN64)

    DWORD convertAccessModeToWndFlag(Mode m) {
        switch (static_cast<int>(m)) {
            case static_cast<int>(Mode::Read):
                return GENERIC_READ;
            case static_cast<int>(Mode::Write):
                return GENERIC_WRITE;
            default:
                return GENERIC_READ | GENERIC_WRITE;
        }
    }

    long long getWndFileSize(HANDLE fileHandle) {
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
        
        if (INVALID_HANDLE_VALUE != fileHandle) {
            length_ = getWndFileSize(fileHandle);
            DWORD bytesRead;
            buf_ = new char[length_];
            const bool readSuccessful = ReadFile(fileHandle, (void *)buf_, length_, &bytesRead, nullptr);
            if (!readSuccessful) {
                std::stringstream errss;
                errss << "Could not read \"" << name_ << "\"\n";
                errss << "Reason: ";
                const WndErrorCode err = GetLastError();
                switch(err) {
                    case ERROR_INSUFFICIENT_BUFFER:
                        errss << "insufficient buffer.";
                        break;
                    case ERROR_INVALID_USER_BUFFER:
                        errss << "invalid user buffer.";
                        break;
                    case ERROR_NOT_ENOUGH_MEMORY:
                        errss << "not enough memory.";
                        break;
                    case ERROR_BROKEN_PIPE:
                        errss << "broken pipe.";
                        break;
                    default:
                        errss << "unknown reason.";
                }

                stx::panic(errss.str());
            }
        }
        else {
            stx::panic("Cannot read file \"" + name_ + "\": no such file or directory.");
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
            length_ = getFileSize(file);
            buf_ = new char[length_];
            const size_t readCount = fread((void *)buf_, sizeof(char), length_, file);
            if (!(fsize == readCount)) {
                stx::panic("File was read partially.\n");
            }
        }
        else {
            stx::panic("An error occured while reading \"" + name_ + "\".");
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
    char File::watch() const {
        return buf_[caret_];
    }

    void File::rewind(size_t offset) {
        if (static_cast<llong>(caret_) - static_cast<llong>(offset) < 0) {
            caret_ = 0;
        }
        else caret_ -= offset;
    }

    File::~File() {
        if (buf_)
            delete[] buf_;
    }

    bool eof(const File &f) {
        return f.caret_ >= f.length_;
    }
} // namespace fs