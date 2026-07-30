#include "defaultRuntime.hpp"
#include "../../../core/process/process.hpp"
#include "../../../core/paths.hpp"

#include <filesystem>
#include <iostream>

DefaultRuntime::DefaultRuntime(Paths &paths) : Runtime(paths) {};

void DefaultRuntime::run(const std::string &file, std::vector<std::string> args)
{
    std::cout << "Creating default runtime for file " << file << std::endl;

    process = new Process(std::filesystem::path(file), args);
    process->start();
    process->wait();
}