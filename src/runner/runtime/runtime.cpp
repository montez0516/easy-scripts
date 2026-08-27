#include "runtime.hpp"

#include "../../core/process/process.hpp"
#include "../../core/paths.hpp"
#include "../../core/eventBus/eventBus.hpp"

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
    process->start();

    Process *processPtr = process.get();
    process->onFinished([processPtr](DWORD exitCode)
                        { 
                            if(exitCode != 0 )
                            {
                                spdlog::error(processPtr->error());
                            } });
    process->readyRead([processPtr]()
                       { std::cout << processPtr->read() << std::endl; });
    // NOTE: REORGANIZE PROCESS CLASS SO THAT THE READYREAD THREADS ONLY START AFTER PROCESS START IS CALLED
    runtimes_.push_back(std::move(process));
}

std::filesystem::path Runtime::executable(const std::filesystem::path &script) const
{
    return "";
}

void Runtime::prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const
{
    args.insert(args.begin(), script.string());
}