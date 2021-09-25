#include "inputreader.hpp"

const auto READ_MODE = fs::Mode::Read;

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