#include <filesystem>

#include "core/main/scriptManager.hpp"

int main()
{
    ScriptManager *manager = new ScriptManager();
    manager->initialize(std::filesystem::path("../scripts"));

    delete manager;
    return 0;
}