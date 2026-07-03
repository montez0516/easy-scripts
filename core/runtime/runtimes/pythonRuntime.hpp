#ifndef PYTHON_R_H
#define PYTHON_R_H

#include "../runtime.hpp"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

class PythonRuntime : public Runtime
{

public:
    void run(const fs::path &file, const std::vector<std::string> &args);
};

#endif