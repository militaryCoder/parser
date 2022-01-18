#pragma once

#include <memory>
#include <string>
#include <vector>

#include "fs/file.hpp"
#include "token.hpp"

template <typename T>
using UniquePtr = std::unique_ptr<T>;
using Str = std::string;
template <typename T>
using Vec = std::vector<T>;
using TokenRawView = Str;

class InputReader
{
    UniquePtr<fs::File> file_;

    char consume();
    char peek();
    char watch();
public:
    InputReader(const char *filename);
    InputReader(const Str &filename);
    InputReader(Str &&filename);
    ~InputReader();

    Vec<TokenRawView> tokenize();
};