#include "scriptManager.hpp"

#include <filesystem>
#include <iostream>

#include "script.hpp"

void ScriptManager::initialize(const std::filesystem::path &scriptFolder)
{
    for (const auto &entry : std::filesystem::directory_iterator(scriptFolder))
    {
        if (entry.is_directory())
        {
            Script script(entry.path());
            scripts.push_back(script);
        }
    }
}