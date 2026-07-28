#include <filesystem>

#include "core/main/scriptManager.hpp"
#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_level(spdlog::level::debug);
    ScriptManager *manager = new ScriptManager();
    manager->initialize(std::filesystem::path("../scripts"));

    delete manager;
    return 0;
}