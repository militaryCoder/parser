#pragma once

#include <string>

using String = std::string;

namespace fs
{
    class Path
    {
        String absolute_;
        bool isDir_;
    public:
        Path(const char *p);
        Path(const String &p);
    
        bool exists() const;
        static auto expand(const String &s) -> String;
        static auto fold(const String &s) -> String;
        auto absolute() const -> String;
        auto relative() const -> String;
    };
}
