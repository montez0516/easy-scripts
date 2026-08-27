#ifndef DEF_RUNTIME_H
#define DEF_RUNTIME_H

#include "../runtime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"
#include "../../../core/eventBus/eventBus.hpp"

#include <vector>
#include <string>

class DefaultRuntime : public Runtime
{
public:
    using Runtime::Runtime;

protected:
    std::filesystem::path executable(const std::filesystem::path &script) const override;
    void prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const override;
};

#endif