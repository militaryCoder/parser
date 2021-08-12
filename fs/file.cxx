#include "file.hxx"

#include "exception.hxx"

namespace fs
{
    File::File(const char *filename, Mode m)
        : name_(filename)
    {
        file_ = fopen(name_, static_cast<const char *>(m));
        if (!this->isValid())
        {
            if (Mode::Read == m)
                throw FileReadError(std::string(filename) + ": no such file or directory.");
            else
                throw BadFileAccess("An error occured while reading " + std::string(filename));
            // TODO: write Filepath class to handle expansion to absolute file names or folding into relative
        }
    }
    
    bool File::isValid() const
    {
        return nullptr != file_;
    }
}
