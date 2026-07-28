#include "scriptManager.hpp"

#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>

#include "spdlog/spdlog.h"
#include "script.hpp"
#include "../process/process.hpp"

ScriptManager::ScriptManager()
{
    pipe = new NamedPipe();
}

bool ScriptManager::initialize(const std::filesystem::path &scriptFolder)
{
    if (!pipe->create())
    {
        spdlog::critical("scriptManager(): NamedPipe failed to create.");
        delete pipe;
        pipe = nullptr;
        return false;
    }

    runner = new Process(std::filesystem::absolute("./runner.exe"), {});
    runner->start();

    for (const auto &entry : std::filesystem::directory_iterator(scriptFolder))
    {
        if (entry.is_directory())
        {
            Script script(entry.path(), this);
            scripts.push_back(script);
        }
    }
    return true;
}

void ScriptManager::run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args)
{
    if (pipe->isNull())
    {
        spdlog::critical("ScriptManager(run): pipe is NULL");
        return;
    }

    pipe->waitForConnection();

    pipe->write(nlohmann::json({{"language", language}, {"file", scriptFile.string()}, {"args", args}}).dump());
}