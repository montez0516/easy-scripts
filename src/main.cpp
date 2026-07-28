#include <filesystem>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

#include "core/main/scriptManager.hpp"

int main()
{
    spdlog::set_level(spdlog::level::debug);
    ScriptManager *manager = new ScriptManager();
    manager->initialize(std::filesystem::path("../scripts"));

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    delete manager;
    return 0;
}