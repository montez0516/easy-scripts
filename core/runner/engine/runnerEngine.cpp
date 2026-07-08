#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "runnerEngine.h"
#include "../runtime/runtimes/pythonRuntime.hpp"
#include "../runtime/runtimes/defaultRuntime.hpp"

std::vector<std::string> split(std::string &str, char delim)
{
    std::vector<std::string> splitted;
    std::stringstream s(str);

    std::string word;

    while (std::getline(s, word, delim))
        splitted.push_back(word);

    return splitted;
}

void Engine::initialize()
{
    runtimeManager->registerRunTime("python", std::make_unique<PythonRuntime>());
    runtimeManager->registerRunTime("default", std::make_unique<DefaultRuntime>());
}

void Engine::run(const std::string &language, const std::string &file, std::string &args)
{
    Runtime *runtime = runtimeManager->getRuntime(language);

    if (runtime == nullptr)
        return;

    std::cout << std::filesystem::absolute(file) << std::endl;

    std::vector<std::string> v_args = split(args, ' ');

    runtime->run(file, v_args);
}