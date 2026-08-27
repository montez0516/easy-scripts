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

std::filesystem::path PythonRuntime::executable(const std::filesystem::path &script) const
{
    std::filesystem::path scriptParent = script.parent_path();
    if (std::filesystem::exists(scriptParent / "venv"))
    {
        return scriptParent / "venv" / "Scripts" / "python.exe";
    }
    return paths_.python();
}

void PythonRuntime::prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const
{
    args.insert(args.begin(), script.string());
    args.insert(args.begin(), "-u");
}