#include "RunnerEngine.h"
#include "../runtime/runtimes/PythonRuntime.h"
#include "../runtime/runtimes/DefaultRuntime.h"
#include "../../core/Paths.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"

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

Engine::Engine(Paths &paths, EventBus &bus) : paths_(paths), bus_(bus)
{
    bus_.subscribe("script.run", [this](const Message &message)
                   {
                       std::string language = message.data.value("language", "default");
                       Runtime *runtime = runtimeManager_.getRuntime(language);

                       if(runtime == nullptr)
                       {
                            spdlog::error("No runtime found for language {}", language);
                            return;
                       }

                       runtime->run(message); });
}

void Engine::initialize()
{
    runtimeManager_.registerRunTime("python", std::make_unique<PythonRuntime>(paths_, bus_));
    runtimeManager_.registerRunTime("default", std::make_unique<DefaultRuntime>(paths_, bus_));
}