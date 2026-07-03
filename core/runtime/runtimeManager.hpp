#ifndef RUNTIME_MANAGER_H
#define RUNTIME_MANAGER_H

#include "runtime.hpp"

#include <memory>
#include <unordered_map>

class RuntimeManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<Runtime>> runtimes;

public:
    void registerRunTime(std::string &language, std::unique_ptr<Runtime>);
    Runtime *getRuntime(std::string &language);
};

#endif