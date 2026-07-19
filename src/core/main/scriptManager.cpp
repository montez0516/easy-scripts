#include "scriptManager.hpp"

#include <filesystem>
#include <iostream>

#include "spdlog/spdlog.h"
#include "script.hpp"
#include "../process/process.hpp"

void ScriptManager::initialize(const std::filesystem::path &scriptFolder)
{
    for (const auto &entry : std::filesystem::directory_iterator(scriptFolder))
    {
        if (entry.is_directory())
        {
            Script script(entry.path(), this);
            scripts.push_back(script);
        }
    }
}

void ScriptManager::run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args)
{
    std::string s_args = std::accumulate(args.begin(), args.end(), std::string(""), [](const std::string &a, const std::string &b)
                                         { return a.empty() ? b : a + " " + b; });

    Process process(std::filesystem::absolute("runner.exe"), {language, std::filesystem::absolute(scriptFile).string()});
    process.start();
    process.wait();
}