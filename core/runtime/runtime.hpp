#ifndef RUNTIME_H
#define RUNTIME

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class Runtime
{
public:
    virtual void run(const fs::path &file, const std::vector<std::string> &args) = 0;
};

#endif