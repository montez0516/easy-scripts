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
    DefaultRuntime(Paths &paths);
    void run(const std::string &file, std::vector<std::string> args) override;
};

#endif