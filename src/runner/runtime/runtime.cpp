#include "runtime.hpp"

#include "../../core/process/process.hpp"
#include "../../core/paths.hpp"
#include "../../core/eventBus/eventBus.hpp"
#include "../../main/scriptManager.hpp"

#include <spdlog/spdlog.h>

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>

Runtime::Runtime(Paths &paths, EventBus &bus) : paths_(paths), bus_(bus)
{
}

void Runtime::run(const std::string &file, std::vector<std::string> args)
{
    spdlog::debug("PYTHON: {}", file);

    fs::path absFile = fs::absolute(file);

    prepareArguments(absFile, args);

    std::unique_ptr<Process> process = std::make_unique<Process>(executable(absFile), args);
    process->setCurrentDirectory(absFile.parent_path());
    process->setCaptureHandles(true);

    Process *processPtr = process.get();
    process->registerReadyReadCallback([this, processPtr]()
                                    { 
                                        std::string scriptPayload = processPtr->read();
                                        spdlog::debug("RUNTIME RECIEVED EVENT {}", scriptPayload);
                                        ScriptEvent<std::string> event;
                                        event.to = "runner.exe";
                                        event.from = "Runtime";
                                        event.payload = scriptPayload;

                                        bus_.publish(event);
                                    });
    process->registerOnFinishedCallback([processPtr](DWORD exitCode)
                                        { 
                            if(exitCode != 0 )
                            {
                                spdlog::error(processPtr->error());
                            } });
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
    bus_.subscribe<ScriptEvent<std::string>>([this](ScriptEvent<std::string> &event)
{
    if(runtimes_.find(event.to) == runtimes_.end())
    {
        spdlog::error("Runtime(registerListener): event reached dead end to={} from={} payload={}", event.to, event.from, event.payload);
        return;
    }
    std::unique_ptr<Process> process = runtimes_[event.to];

    process->write(event.payload);

    
});
}