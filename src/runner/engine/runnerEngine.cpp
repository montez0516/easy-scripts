#include "runnerEngine.hpp"
#include "../runtime/runtimes/pythonRuntime.hpp"
#include "../runtime/runtimes/defaultRuntime.hpp"
#include "../../core/paths.hpp"

#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>

std::vector<std::string> split(std::string &str, char delim)
{
    std::vector<std::string> splitted;
    std::stringstream s(str);

    std::string word;

    while (std::getline(s, word, delim))
        splitted.push_back(word);

    return splitted;
}

Engine::Engine(Paths &paths) : paths_(paths) {};

void Engine::initialize()
{
    runtimeManager->registerRunTime("python", std::make_unique<PythonRuntime>(paths_));
    runtimeManager->registerRunTime("default", std::make_unique<DefaultRuntime>(paths_));
}

void Engine::run(const std::string &language, const std::string &file, std::string &args)
{
#if defined(BUILD_DEV)
    spdlog::debug("FINDING RUNTIME FOR LANGUAGE");
#endif
    Runtime *runtime = runtimeManager->getRuntime(language);

    if (runtime == nullptr)
    {
        spdlog::error("No runtime found for language {}", language);
        return;
    }

    std::vector<std::string> v_args = split(args, ' ');

    runtime->run(file, v_args);
}