#include "pythonRuntime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

// const fs::path PYTHON = fs::path("../bin") / "python" / "python.exe";

PythonRuntime::PythonRuntime(Paths &paths) : Runtime(paths) {};

void PythonRuntime::run(const std::string &file, std::vector<std::string> args)
{
#if defined(BUILD_DEV)
    spdlog::debug("Creating python runtime for file {}", file);
#endif

    fs::path abs_file = fs::absolute(file);
    fs::path parent_dir = fs::absolute(file).parent_path();

    args.insert(args.begin(), abs_file.string());

    fs::path exe = paths_.python();

    if (fs::exists(parent_dir / "venv"))
        exe = parent_dir / "venv" / "Scripts" / "python.exe";

    spdlog::info("{} {}", exe.string(), abs_file.string());

    process = new Process(exe, args);
    process->setCurrentDirectory(parent_dir.wstring());
    process->start();
}