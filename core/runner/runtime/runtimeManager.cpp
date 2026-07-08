#include "runtimeManager.hpp"

#include "runtime.hpp"

#include <string>
#include <memory>

void RuntimeManager::registerRunTime(const std::string &language, std::unique_ptr<Runtime> runtime)
{
    runtimes.emplace(language, std::move(runtime));
}

Runtime *RuntimeManager::getRuntime(const std::string &language)
{
    auto it = runtimes.find(language);

    if (it == runtimes.end())
        return nullptr;

    return it->second.get();
}