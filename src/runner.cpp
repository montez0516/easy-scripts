#include <filesystem>
#include <string>
#include <iostream>

#include "runner/engine/runnerEngine.hpp"
#include "spdlog/spdlog.h"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    spdlog::info("Runner.exe {}", argc);

    Engine *engine = new Engine;
    engine->initialize();

    std::string language;
    std::string file;
    std::string args;

    if (argc < 3)
    {
        std::cerr << "Not enough arguments detected" << std::endl;

        for (int i = 0; i < argc; i++)
        {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;
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