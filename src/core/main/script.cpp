#include "script.hpp"

#include <utility>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include "scriptManager.hpp"

using json = nlohmann::json;

Script::Script(std::filesystem::path scriptDir, ScriptManager *m) : dir(std::move(scriptDir)), manager(m)
{
    initialize();
};

void Script::initialize()
{
    parseInfo();

    if (info.metaData["type"] == "service")
    {
        run({});
    }
}

void Script::parseInfo()
{
    std::ifstream metaFile(dir / "meta.json");

    info.metaData = json::parse(metaFile);

    std::string language = info.metaData["language"];

    std::string file = "main.exe";

    if (language == "python")
        file = "main.py";

    info.scriptFile = dir / file;
    info.icon = dir / "icon.png";

    spdlog::info("{}: \n{}\n{}\n{}\n{}\n", info.metaData["name"].get<std::string>(), dir.string(), info.scriptFile.string(), info.icon.string(), info.metaData.dump());
}

void Script::run(std::vector<std::string> args)
{
    if (!std::filesystem::exists(info.scriptFile))
        return;
    manager->run(info.metaData["language"], info.scriptFile, args);
}