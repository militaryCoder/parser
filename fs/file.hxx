#pragma once

#include <string>

namespace fs
{
    using Str = std::string;
    enum class Mode
    {
        Read,
        Write
    };

    class File
    {
        Str name_;
        size_t length_;
        char *buf_ = nullptr;
        size_t caretPos_;
    public:
        File(const char *filename, Mode);
        File(const Str &filename, Mode);
        char readChar();
        Str readLine();
        bool isValid() const;
        ~File();
    };
}
