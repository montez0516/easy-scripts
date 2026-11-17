#include "Runtime.h"

#include "../../core/Paths.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"
#include "../../core/process/ProcessHandler.h"

#include <spdlog/spdlog.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>
#include <sstream>
#include <chrono>

Runtime::Runtime(Paths &paths, EventBus &bus) : paths_(paths), bus_(bus)
{
    registerListener();
}

Runtime::~Runtime()
{
    shutdown();
}
void Runtime::run(const Message &message)
{
    nlohmann::json messageData = message.data;

    std::string name = message.scriptName;
    std::string file = messageData.at("file");
    std::vector<std::string> args = messageData.value<std::vector<std::string>>("args", {});

    fs::path absFile = fs::absolute(file);

    prepareArguments(absFile, args);

    std::unique_ptr<ProcessHandler> process = std::make_unique<ProcessHandler>(executable(absFile), args);

    process->setCurrentDirectory(absFile.parent_path());
    process->setCapturePipes(true);

    ProcessHandler *processPtr = process.get();

    process->registerStdoutMessageReader([this, name](const Message &message)
                                         { bus_.publish("script.event", message); });

    process->registerOnFinishedCallback([this, name, processPtr](DWORD code)
                                        {
                                            std::string errorMsg = processPtr->error();
                                            bus_.publish("script.event", {.type = "scripts.state",
                                                                          .scriptName = name,
                                                                          .data = {
                                                                              {"state", code != 0 ? "scripts.error" : "scripts.finished"},
                                                                              {"code", code},
                                                                              {"error", errorMsg}}});
                                            runtimes_.erase(name); });

    if (!process->start())
    {
        bus_.publish("script.event", {.type = "scripts.state", .scriptName = name, .data = {{"state", "scripts.error"}, {"error", "Failed to start process"}}});
        return;
    }

    runtimes_[name] = (std::move(process));
    bus_.publish("script.event", {.type = "scripts.state", .scriptName = name, .data = {{"state", "scripts.started"}}});
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
    bus_.subscribe("runtime.response", [this](const Message &message)
                   {
                       ProcessHandler *process = getRuntimeProcess(message.scriptName);
                       if (process == nullptr)
                           return;
                       process->write(message); });
    bus_.subscribe("runtime.shutdown", [this](const Message &message)
                   { shutdown(); });
    bus_.subscribe("scripts.stop", [this](const Message &message)
                   {
                       ProcessHandler *process = getRuntimeProcess(message.scriptName);
                       if (process == nullptr)
                           return;

                       if (!process->shutdown())
                           process->terminate();
                       runtimes_.erase(message.scriptName); });
}

ProcessHandler *Runtime::getRuntimeProcess(const std::string &name)
{
    if (runtimes_.find(name) == runtimes_.end())
        return nullptr;

    return runtimes_.at(name).get();
}

void Runtime::shutdown()
{

    for (const auto &[id, runtime] : runtimes_)
    {
        runtime->write({.type = "scripts.shutdown"});
    }

    for (const auto &[id, runtime] : runtimes_)
    {
        if (!runtime->waitFor(std::chrono::milliseconds(3000)))
        {
            runtime->terminate();
        }
        runtimes_.erase(id);
    }
}

std::string Runtime::name() const
{
    return "constructor";
}