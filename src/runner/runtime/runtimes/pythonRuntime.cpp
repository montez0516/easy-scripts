#include "pythonRuntime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <memory>

namespace fs = std::filesystem;

PythonRuntime::PythonRuntime(Paths &paths) : Runtime(paths) {};

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

    process = std::make_unique<Process>(exe, args);
    process->setCurrentDirectory(parentDir.wstring());
    process->start();
    process->onFinished([this, &file](DWORD exitCode)
                        { 
                            if(exitCode != 0 )
                            {
                                spdlog::error(process->error());
                            } });
    process->readyRead([this]()
                       { std::cout << process->read() << std::endl; });
}