#pragma once

#include <string>

#include "file.hpp"

using Str = std::string;
class InputReader
{
    fs::File *file_;
public:
    InputReader(const char *filename);
    InputReader(const Str &filename);
    InputReader(Str &&filename);
    ~InputReader();

    char consume();
    char peek();
};
