#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <filesystem>
#include <vector>

#include "script.hpp"

class ScriptManager
{
private:
    std::vector<Script> scripts;

public:
    void initialize(const std::filesystem::path &ScriptFolder);
};

#endif