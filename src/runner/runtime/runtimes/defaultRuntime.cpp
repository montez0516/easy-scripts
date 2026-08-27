#include "defaultRuntime.hpp"
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

std::filesystem::path DefaultRuntime::executable(const std::filesystem::path &script) const
{
    return "";
}

void DefaultRuntime::prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const
{
    args.insert(args.begin(), script.string());
}