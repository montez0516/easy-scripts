#include "pythonRuntime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"
#include "../../../core/eventBus/eventBus.hpp"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <memory>

namespace fs = std::filesystem;

PythonRuntime::PythonRuntime(Paths &paths, EventBus &bus) : Runtime(paths, bus){
    exe = paths.python();
};

void PythonRuntime::run(const std::string &file, std::vector<std::string> args)
{
    spdlog::debug("PYTHON: {}", file);

    fs::path absFile = fs::absolute(file);
    fs::path parentDir = absFile.parent_path();

    args.insert(args.begin(), absFile.string());
    args.push_back("-u");

    fs::path exe = paths_.python();

    if (fs::exists(parentDir / "venv"))
        exe = parentDir / "venv" / "Scripts" / "python.exe";

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