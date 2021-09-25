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
        size_t length_;
    public:
        File(const char *filename, Mode &&);
        File(const Str &filename, Mode &&);
        char readChar();
        char peekChar();
        char watch() const;
        void rewind(size_t offset);
        Str readLine();
        ~File();

        friend bool eof(const File &);
    };

    bool eof(const File &);
}