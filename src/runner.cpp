#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

#include "runner/engine/runnerEngine.hpp"
#include "spdlog/spdlog.h"
#include "core/ipc/namedPipe.hpp"

namespace fs = std::filesystem;

int main()
{
    NamedPipe *pipe = new NamedPipe();

    if (!pipe->connect(""))
    {
        spdlog::critical("runner(main): NamedPipe failed to connect.");
        return 1;
    }

    Engine *engine = new Engine;
    engine->initialize();

    std::ofstream file("file.txt");

    std::thread([pipe, engine]()
                {
        while(true)
        {
            std::string payload = pipe->read();
            spdlog::info("runner(main): Message from main application in runner: {}", payload);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } });

    return 0;
}