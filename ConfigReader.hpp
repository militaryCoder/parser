#pragma once

#include <vector>
#include <string>

#include <stdio.h>

enum class TokenType {
    ParameterName,
    ParameterValue,
    Separator,
    Number
};

struct Token {
    Token(const std::string &text, TokenType type)
        : text(text), type(type) {}
    std::string text;
    TokenType type;
};

class ConfigReader {
public:
    ConfigReader(const char *filename);
    auto read() -> std::vector<Token>;
    ~ConfigReader();
private:
    FILE *confFile_;
};
