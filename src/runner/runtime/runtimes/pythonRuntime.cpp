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
    spdlog::debug("Creating python runtime for file {}", file);

    fs::path abs_file = fs::absolute(file);
    fs::path parent_dir = fs::absolute(file).parent_path();

    args.insert(args.begin(), abs_file.string());

    fs::path exe = paths_.python();

    if (fs::exists(parent_dir / "venv"))
        exe = parent_dir / "venv" / "Scripts" / "python.exe";

    spdlog::debug("{} {}", exe.string(), abs_file.string());

    process = std::make_unique<Process>(exe, args);
    process->setCurrentDirectory(parent_dir.wstring());
    process->start();
    process->onFinished([](DWORD exitCode)
                        { std::cout << "Python script finished with exit code " << exitCode << std::endl; });
    process->readyRead([this]()
                       { std::cout << process->read() << std::endl; });
}