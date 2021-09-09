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
    public:
        File(const char *filename, Mode);
        File(const Str &filename, Mode);
        char readChar();
        Str readLine();
        bool isValid() const;
        ~File() {};
    };
}
