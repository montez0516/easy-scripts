#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "core/ipc/namedPipe.hpp"
#include "runner/engine/runnerEngine.hpp"

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
            nlohmann::json payload = pipe->json();
            std::string temp = "";
            engine->run(payload["language"], payload["file"], temp);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } })
        .join();

    return 0;
}