#include "defaultRuntime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>
#include <memory>

DefaultRuntime::DefaultRuntime(Paths &paths) : Runtime(paths) {};

void DefaultRuntime::run(const std::string &file, std::vector<std::string> args)
{
    spdlog::debug("DEFAULT RUNTIME: {}", file);

    fs::path absFile = fs::absolute(file);
    fs::path parentDir = absFile.parent_path();

    args.insert(args.begin(), absFile.string());
    args.push_back("-u");
    
    std::unique_ptr<Process> process = std::make_unique<Process>(exe, args);
    process->setCurrentDirectory(parentDir.wstring());
    process->start();
    process->onFinished([&process](DWORD exitCode)
                        { 
                            if(exitCode != 0 )
                            {
                                spdlog::error(process->error());
                            } });
    process->readyRead([&process]()
                       { std::cout << process->read() << std::endl; });
    // NOTE: REORGANIZE PROCESS CLASS SO THAT THE READYREAD THREADS ONLY START AFTER PROCESS START IS CALLED
    runtimes_.push_back(process);
}