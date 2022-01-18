#include "inputreader.hpp"

InputReader::InputReader(const char *filename)
    : file_(std::make_unique<fs::File>(filename, fs::Mode::Read)) {}

InputReader::InputReader(const Str &filename)
    : file_(std::make_unique<fs::File>(filename, fs::Mode::Read)) {}

InputReader::InputReader(Str &&filename)
    : file_(std::make_unique<fs::File>(std::move(filename), fs::Mode::Read)) {}

InputReader::~InputReader() {}

char InputReader::consume() {
    return file_->readChar();
}

char InputReader::peek() {
    return file_->peekChar();
}
char InputReader::watch() {
    return file_->watch();
}

Vec<TokenRawView> InputReader::tokenize() {
    const char SPACE = ' ';

    Vec<TokenRawView> outSeq;
    Str tokenBuf;
    while (!eof(*file_)) {
        // getting rid of trailing spaces
        const char curr = consume();
        if (std::isalpha(curr)) {

        }
    }

    return outSeq;
}