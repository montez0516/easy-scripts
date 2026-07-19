#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <filesystem>
#include <vector>

#include "../process/process.hpp"
#include "script.hpp"

class ScriptManager
{
private:
    std::vector<Script> scripts;
    std::vector<Process> runtimes;

public:
    void initialize(const std::filesystem::path &ScriptFolder);
    void run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args);
    void stop();
};

#endif