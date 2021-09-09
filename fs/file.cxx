#include "file.hxx"

#include <string.h>

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#endif

namespace fs
{
#if defined(_WIN32) || defined(_WIN64)

    File::File(const char *filename, Mode m)
        : name_(filename)
    {
        const DWORD NON_SHARABLE = 0;
        const char *mode = m == Mode::Read ? "r" : "w";
        const auto openMode = 0 == strcmp("r", mode) ? GENERIC_READ : GENERIC_WRITE;
        HANDLE fileHandle = CreateFile(filename,
                                       openMode,
                                       NON_SHARABLE,
                                       nullptr,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       nullptr);
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
