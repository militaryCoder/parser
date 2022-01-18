#include "ConfigReader.hpp"

ConfigReader::ConfigReader(const char *filename) {
    confFile_ = fopen(filename, "r");
}
ConfigReader::auto read() -> std::vector<Token> {
    std::vector<Token> tokens;
    while (!feof(confFile_)) {
        char nextch = (char)fgetc(confFile_);
        if (nextch != ' ') {
            std::string tokenText;
            if (isalpha(nextch)) {
                while (isalpha(nextch)) {
                    tokenText += nextch;
                    nextch = (char)fgetc(confFile_);
                }
                if (tokens.size() != 0 && tokens.back().type == TokenType::Separator)
                    tokens.emplace_back(tokenText, TokenType::ParameterValue);
                else tokens.emplace_back(tokenText, TokenType::ParameterName);
            }
            if (nextch == ':')
                tokens.emplace_back(std::string(1, nextch), TokenType::Separator);
        }
    }

    return tokens;
}
ConfigReader::~ConfigReader() {
    if (confFile_)
        fclose(confFile_);
}
