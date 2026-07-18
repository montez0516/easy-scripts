#include <filesystem>
#include <string>
#include <iostream>
#include <source_location>

#include "core/runner/runtime/runtimeManager.hpp"
#include "core/runner/engine/runnerEngine.h"
#include "spdlog/spdlog.h"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    spdlog::info("TEST FIRST PRINT {}", std::source_location::current());
    Engine *engine = new Engine;
    engine->initialize();

    std::string language;
    std::string file;
    std::string args;

    if (argc < 3)
    {
        std::cerr << "Not enough arguments detected" << std::endl;
        return 1;
    }
    language = argv[1];
    file = argv[2];

    if (argc >= 4)
        args = argv[3];

    std::cout << "Running plugin " << language << " " << file << " " << args << std::endl;

    engine->run(language, file, args);
    return 0;
}