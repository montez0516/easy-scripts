#ifndef PYTHON_R_H
#define PYTHON_R_H

#include "../runtime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"
#include <vector>

class PythonRuntime : public Runtime
{
private:
    Process *process;
public:
    PythonRuntime(Paths &paths);
    void run(const std::string &file, std::vector<std::string> args) override;
};

#endif