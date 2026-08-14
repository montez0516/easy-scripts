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

    if (metaData["type"] == "service")
    {
        spdlog::debug("Running service {}", metaData.value<std::string>("name", ""));
        run({});
    }
}

void Script::parseInfo()
{
    loadMetaData();
    findScriptFile();
    logInfo();
}

void Script::loadMetaData()
{

    std::filesystem::path metaPath = dir / "meta.json";

    if (!std::filesystem::exists(metaPath))
    {
        metaData["name"] = dir.stem();
        return;
    }

    std::ifstream metaFile(metaPath);
    metaData = nlohmann::json::parse(metaFile);
    metaFile.close();
}

void Script::findScriptFile()
{
    std::vector<std::string> script_extensions = {".py", ".exe"};

    for (const auto &ext : script_extensions)
    {
        std::filesystem::path filename = dir / ("main" + ext);
        if (std::filesystem::exists(filename))
            scriptFile = filename;
    }
}

void Script::logInfo()
{
    spdlog::debug("{}: {} {} \n{}\n", metaData.value<std::string>("name", ""), dir.string(), scriptFile.string(), metaData.dump(2));
}

void Script::run(std::vector<std::string> args)
{
    if (!std::filesystem::exists(scriptFile))
        return;
    manager->run(metaData["language"], scriptFile, args);
}