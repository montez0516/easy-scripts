#include "pythonRuntime.hpp"
#include "../../../process/process.hpp"

#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

const fs::path PYTHON = fs::path("../bin") / "python" / "python.exe";

void PythonRuntime::run(const std::string &file, std::vector<std::string> args)
{
    std::cout << "Creating python runtime for file " << file << std::endl;

    args.insert(args.begin(), fs::absolute(file).string());

    process = new Process(PYTHON, args);
    process->start();
    std::cout << process->read() << std::endl;
    process->wait();
}