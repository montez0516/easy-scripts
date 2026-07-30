#ifndef RUNTIME_H
#define RUNTIME_H

#include "../../core/process/process.hpp"
#include "../../core/paths.hpp"

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class Runtime
{
private:
    Process *process;
protected:
    Paths &paths_;
public:
    Runtime(Paths &paths) : paths_(paths) {};
    virtual void run(const std::string &file, std::vector<std::string> args) = 0;
};

#endif