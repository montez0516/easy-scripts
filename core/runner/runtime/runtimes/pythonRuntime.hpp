#ifndef PYTHON_R_H
#define PYTHON_R_H

#include "../runtime.hpp"
#include "../../../process/process.hpp"
#include <vector>

class PythonRuntime : public Runtime
{
private:
    Process *process;

public:
    void run(const std::string &file, std::vector<std::string> args) override;
};

#endif