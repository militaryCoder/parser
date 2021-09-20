#include "inputreader.hpp"

const auto READ_MODE = fs::Mode::Read;

InputReader::InputReader(const char *filename)
    : file_(new fs::File(filename, READ_MODE)) {}
InputReader::InputReader(const Str &filename)
    : file_(new fs::File(filename, READ_MODE)) {}
InputReader::InputReader(Str &&filename)
    : file_(new fs::File(std::move(filename), READ_MODE)) {}

char InputReader::consume() {
    return file_->readChar();
}

char InputReader::peek() {
    return file_->peekChar();
}