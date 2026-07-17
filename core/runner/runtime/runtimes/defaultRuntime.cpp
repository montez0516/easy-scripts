#include <filesystem>

#include "defaultRuntime.hpp"
#include "../../../process/process.hpp"
#include "iostream"

void DefaultRuntime::run(const std::string &file, std::vector<std::string> args)
{
    std::cout << "Creating default runtime for file " << file << std::endl;

    process = new Process(std::filesystem::path(file), args);
    process->start();
}