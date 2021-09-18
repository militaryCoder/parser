#include "path.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if defined(_WIN32) || defined(_WIN64)
    #define FILESTATS _stat
#elif defined(__unix__) || defined(__linux__)
    #define FILESTATS stat
#endif

namespace fs
{
    bool isDirectory(const String &path)
    {
        struct FILESTATS fileStats;
        const int statRet = stat(path.c_str(), &fileStats);
        if (-1 == statRet)
            throw BadFileAccess("Cannot read " + path + " properties.");

        return (fileStats.st_mode & S_IFMT) == S_IFDIR;
    }

    const size_t MAX_PATH = 1024u;

    String cwd()
    {
        char buf[MAX_PATH] = {};

        if (!getcwd(buf, MAX_PATH))
            throw BadFileAccess("Cannot get current working directory path.");
        
        return String(std::begin(buf), std::end(buf));
    }

    bool isAbsolute(const String &path)
    {
        return String::npos != path.find(cwd());
    }

    Path::Path(const char *p)
    {
        const String path = String(p);
        absolute_ = isAbsolute(path) ? path : cwd() + path;
        isDir_ = isDirectory(absolute_);
    }

    Path::Path(const String &p)
    {
        absolute_ = isAbsolute(p) ? p : cwd() + p;
        isDir_ = isDirectory(absolute_);
    }

    bool Path::exists() const
    {
        struct FILESTATS fstats;
        return (0 == stat(absolute_, fstats));
    }
    
    auto Path::absolute() const -> String
    {
        return absolute_;
    }
    auto Path::relative() const -> String
    {
        return absolute_.substr(cwd().size());
    }

    static auto expand(const String &s) -> String
    {
        return Path(s).absolute();
    }
    static auto fold(const String &s) -> String
    {
        return Path(s).relative();
    }
}
