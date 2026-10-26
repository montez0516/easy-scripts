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

    spdlog::debug("PYTHON: {}", file);

    fs::path absFile = fs::absolute(file);

    prepareArguments(absFile, args);

    std::unique_ptr<Process> process = std::make_unique<Process>(executable(absFile), args);
    process->setCurrentDirectory(absFile.parent_path());
    process->setCaptureHandles(true);

    process->registerReadyReadCallback([this, name](const Message &message)
                                       { bus_.publish("script.event", message); });

    Process *processPtr = process.get();
    process->registerOnFinishedCallback([this, name, processPtr](DWORD code)
                                        {
                                            std::string errorMsg = processPtr->error();
                                            bus_.publish("script.event", {.type = "scripts.state",
                                                                          .scriptName = name,
                                                                          .data = {
                                                                              {"state", code != 0 ? "scripts.error" : "scripts.finished"},
                                                                              {"code", code},
                                                                              {"error", errorMsg}}}); });

    process->start();

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
        Process *process = getRuntimeProcess(message.scriptName);
        process->write(message); });
    bus_.subscribe("runtime.shutdown", [this](const Message &message)
                   { shutdown(); });
    bus_.subscribe("scripts.stop", [this](const Message &message)
                   {
                       Process *process = getRuntimeProcess(message.scriptName);
                       if (process == nullptr)
                           return;

                       process->write({.type = "scripts.shutdown"});

                       if (!process->waitFor(std::chrono::milliseconds(3000)))
                           process->terminate(); });
}

Process *Runtime::getRuntimeProcess(const std::string &name)
{
    if (runtimes_.find(name) == runtimes_.end())
        return {};

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
    }
}

std::string Runtime::name() const
{
    return "constructor";
}