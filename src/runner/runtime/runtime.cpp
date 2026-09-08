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
#include <sstream>

Runtime::Runtime(Paths &paths, EventBus &bus) : paths_(paths), bus_(bus)
{
    registerListener();
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
    process->registerReadyReadCallback([this, processPtr, file]()
                                       { 
                                        std::string scriptPayload = processPtr->read();
                                        
                                        std::istringstream is(scriptPayload);

                                        spdlog::debug("RUNTIME: Handling process output {}", scriptPayload);
                                        std::string line;

                                        while(std::getline(is, line)){
                                            spdlog::debug("RUNTIME: OUTPUT {}", line);
                                            ScriptEvent<std::string> event;
                                            event.to = "runner.exe";
                                            event.from = file;
                                            event.payload = line;
                                            event.type = "api";
                                            bus_.publish(event);
                                        } });
    process->registerOnFinishedCallback([this, processPtr, absFile](DWORD exitCode)
                                        { 
                        spdlog::debug("Runtime(registerOnFinishedCallback): {} exited with code {}", absFile.string(), exitCode);
                        ScriptEvent<std::string> event;
                        event.to = "runner.exe";
                        event.from = absFile.parent_path().filename().string();
                        event.payload = "";
                        event.type = "finished";
                        bus_.publish(event); });
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
                                                for(const auto &pair : runtimes_)
                                                {
                                                    spdlog::debug(pair.first);
                                                }

                                                 if (runtimes_.find(event.to) == runtimes_.end())
                                                 {
                                                     return;
                                                 }

                                                Process *process = runtimes_[event.to].get();
                                                process->write(event.payload); });
}