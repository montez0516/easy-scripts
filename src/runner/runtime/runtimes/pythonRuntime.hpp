#ifndef PYTHON_R_H
#define PYTHON_R_H

#include "../runtime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"
#include "../../../core/eventBus/eventBus.hpp"
#include <vector>
#include <memory>

class PythonRuntime : public Runtime
{
public:
    PythonRuntime(Paths &paths, EventBus &bus);
    void run(const std::string &file, std::vector<std::string> args) override;
};

#endif