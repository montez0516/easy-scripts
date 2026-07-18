#include "core/main/scriptManager.hpp"
#include <filesystem>

int main()
{
    ScriptManager *manager = new ScriptManager();
    manager->initialize(std::filesystem::path("../scripts"));

    return 0;
}