#include "defaultRuntime.hpp"
#include "../../../process/process.hpp"
#include <filesystem>

void DefaultRuntime::run(const std::string &file, std::vector<std::string> &args)
{
    process = new Process(std::filesystem::path(file), args);
    process->start();
}