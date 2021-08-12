#pragma once

#include <stdio.h>

namespace fs
{
    enum class Mode : const char *
    {
        Read = "r",
        Write = "w"
    };

    class File
    {
        FILE *file_;
        std::string name_;
    public:
        File(const char *filename, Mode);
        File(const std::string &filename, Mode);
        char readChar();
        std::string readLine();
        bool isOpened() const;
        bool isValid() const;
        ~File();
    };
}
