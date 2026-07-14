#ifndef RUNTIME_H
#define RUNTIME_H

#include <filesystem>
#include <vector>
#include "../../process/process.hpp"

namespace fs = std::filesystem;

class Runtime
{
private:
    Process *process;

public:
    virtual void run(const std::string &file, std::vector<std::string> &args) = 0;
};

#endif