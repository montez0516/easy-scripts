#include "script.hpp"

#include <utility>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

using json = nlohmann::json;

Script::Script(std::filesystem::path scriptDir) : dir(std::move(scriptDir))
{
    parseInfo();
};

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

    std::filesystem::path env = dir / "env";

    if (std::filesystem::exists(env))
    {
        info.hasEnv = true;
    }

    spdlog::info("{}: \n{}\n{}\n{}\n{}\n", info.metaData["name"].get<std::string>(), dir.string(), info.scriptFile.string(), info.icon.string(), info.metaData.dump());
}