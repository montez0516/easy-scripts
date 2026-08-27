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
    using Runtime::Runtime;

protected:
    std::filesystem::path executable(const std::filesystem::path &script) const override;
    void prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const override;
};

#endif