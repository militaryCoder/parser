#pragma once

#include <string>

namespace fs
{
    using Str = std::string;
    enum class Mode
    {
        Read,
        Write,
        ReadWrite
    };

    class File
    {
        Str name_;
        char *buf_ = nullptr;
        size_t caret_ = 0;
    public:
        File(const char *filename, Mode &&);
        File(const Str &filename, Mode &&);
        char readChar();
        char peekChar();
        Str readLine();
        ~File();
    };
}
