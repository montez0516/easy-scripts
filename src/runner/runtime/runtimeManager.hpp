#pragma once

#include "runtime.hpp"

#include <memory>
#include <unordered_map>

class RuntimeManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<Runtime>> runtimes;

public:
    void registerRunTime(const std::string &language, std::unique_ptr<Runtime>);
    Runtime *getRuntime(const std::string &language);
};
