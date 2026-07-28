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
    std::filesystem::path metaPath = dir / "meta.json";
    if (std::filesystem::exists(metaPath))
    {
        try

        {
            std::ifstream metaFile(metaPath);

            info.metaData = json::parse(metaFile);

            metaFile.close();
        }
        catch (const json::type_error &e)
        {
            spdlog::error("Script(parseInfo): Failed to parse meta json: {}\n", e.what());
        }
    }
    else
    {
        spdlog::debug("Script(parseInfo): no metadata file found: {}\n", metaPath.string());
        info.metaData = json();
        info.metaData["name"] = dir.stem();
    }

    std::vector<std::string> script_extensions = {".py", ".exe"};

    for (const auto &ext : script_extensions)
    {
        std::filesystem::path filename = dir / ("main" + ext);
        if (std::filesystem::exists(filename))
            info.scriptFile = filename;
    }

    spdlog::info("{}: \n{}\n{}\n{}\n", info.metaData.value<std::string>("name", ""), dir.string(), info.scriptFile.string(), info.metaData.dump());
}

void Script::run(std::vector<std::string> args)
{
    if (!std::filesystem::exists(info.scriptFile))
        return;
    manager->run(info.metaData["language"], info.scriptFile, args);
}