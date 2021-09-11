#include "file.hxx"

#include <cstdint>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
    using WndErrorCode = DWORD;
#endif

namespace fs
{
#if defined(_WIN32) || defined(_WIN64)

    File::File(const char *filename, Mode m)
        : name_(filename)
    {
        const DWORD NON_SHARABLE = 0;
        const auto openMode = Mode::Read == m ? GENERIC_READ : GENERIC_WRITE;
        HANDLE fileHandle = CreateFile(filename,
                                       openMode,
                                       NON_SHARABLE,
                                       nullptr,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       nullptr);
        
        if (nullptr != fileHandle)
        {
            LARGE_INTEGER fileSize;
            GetFileSizeEx(fileHandle, &fileSize);
            const long long llsize = fileSize.QuadPart;
            DWORD bytesRead;
            buf_ = new char[llsize];
            const bool readSuccessful = ReadFile(fileHandle, (void *)buf_, llsize, &bytesRead, nullptr);
            if (!readSuccessfully)
            {
                std::cerr << "Could not read \"" << name_ << "\"\n";
                std::cerr << "Read " << bytesRead << "bytes.\n";
                std::cerr << "Reason: ";
                const WndErrorCode err = GetLastError();
                switch(err)
                {
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
            caretPos_ = 0;
        }
    }

    File::~File()
    {
        if (buf_)
            delete[] buf_;
    }

#else

    File::File(const char *filename, Mode m)
        : name_(filename)
    {
        file_ = fopen(name_, static_cast<const char *>(m));
    }
    
    bool File::isValid() const
    {
        return nullptr != file_;
    }

    char readChar()
    {
        const int read = fgetc(file_);
        if (EOF != read)
            return static_cast<char>(read);
        else if (feof(file_))
            throw EOFException("EOF reached.");
        else throw BadFileAccessException("Unknown error occured while reading file.");
    }

#endif
}
