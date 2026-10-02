#include "runtime.hpp"

#include "../../core/process/process.hpp"
#include "../../core/paths.hpp"
#include "../../core/eventBus/eventBus.hpp"
#include "../../main/scriptManager.hpp"
#include "../../core/communication/message.hpp"

#include <spdlog/spdlog.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>
#include <sstream>

Runtime::Runtime(Paths &paths, EventBus &bus) : paths_(paths), bus_(bus)
{
    registerListener();
}

void Runtime::run(const Message &message)
{
    nlohmann::json messageData = message.data;

    std::string id = message.id;
    std::string file = messageData.at("file");
    std::vector<std::string> args = messageData.value<std::vector<std::string>>("args", {});

    spdlog::debug("PYTHON: {}", file);

    fs::path absFile = fs::absolute(file);

    prepareArguments(absFile, args);

    std::unique_ptr<Process> process = std::make_unique<Process>(executable(absFile), args);
    process->setCurrentDirectory(absFile.parent_path());
    process->setCaptureHandles(true);

    process->registerReadyReadCallback([this, id](const Message &message)
                                       { bus_.publish("script.event", message); });

    process->registerOnFinishedCallback([this, message](DWORD code)
                                        { bus_.publish("script.event", {.type = MessageType::Event, .name = "script.finished", .id = message.id, .data = code}); });

    process->start();

    runtimes_[file] = (std::move(process));
}

std::filesystem::path Runtime::executable(const std::filesystem::path &script) const
{
    return "";
}

void Runtime::prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const
{
    args.insert(args.begin(), script.string());
}

void Runtime::registerListener()
{
    bus_.subscribe("script.response", [this](const Message &message)
                   {
        Process *process = getRuntime(message.id);
        process->write(message); });
}

Process *Runtime::getRuntime(const std::string &name)
{
    if (runtimes_.find(name) == runtimes_.end())
        return {};

    return runtimes_.at(name).get();
}