#include "runtimeManager.hpp"

#include "runtime.hpp"

#include <string>
#include <memory>
#include <iostream>

void RuntimeManager::registerRunTime(const std::string &language, std::unique_ptr<Runtime> runtime)
{
    runtimes.emplace(language, std::move(runtime));
}

Runtime *RuntimeManager::getRuntime(const std::string &language)
{
    auto it = runtimes.find(language);

    if (it == runtimes.end())
    {
        std::cerr << "Failed to find runtime " << language << std::endl;
        return nullptr;
    }

    return it->second.get();
}