#include "script.hpp"
#include "scriptManager.hpp"
#include "../core/eventBus/eventBus.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <utility>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using json = nlohmann::json;



Script::Script(std::filesystem::path scriptDir, ScriptManager &m, EventBus &bus) : dir_(std::move(scriptDir)), manager_(m), bus_(bus) 
{
    initialize();
};

void Script::initialize()
{
    parseInfo();
}

void Script::parseInfo()
{
    loadMetaData();
    findScriptFile();
}

void Script::loadMetaData()
{

    std::filesystem::path metaPath = dir_ / "meta.json";

    if (!std::filesystem::exists(metaPath))
    {
        metaData_["name"] = dir_.stem();
        return;
    }

    std::ifstream metaFile(metaPath);
    metaData_ = nlohmann::json::parse(metaFile);
    metaFile.close();
}

void Script::findScriptFile()
{
    std::vector<std::string> script_extensions = {".py", ".exe"};

    for (const auto &ext : script_extensions)
    {
        std::filesystem::path filename = dir_ / ("main" + ext);
        if (std::filesystem::exists(filename))
            scriptFile_ = filename;
    }
}

void Script::logInfo()
{
    spdlog::debug("{}: {} {} \n{}\n", metaData_.value<std::string>("name", ""), dir_.string(), scriptFile_.string(), metaData_.dump(2));
}

void Script::run(std::vector<std::string> args)
{
    if (!std::filesystem::exists(scriptFile_))
        return;
    ScriptEvent<nlohmann::json> event;
    event.to = "0";
    event.from = metaData_["name"];
    event.type = "run";
    event.payload = {{"file", scriptFile_}, {"language", metaData_["language"]}, {"args",args}};

    bus_.publish(event);
}

bool Script::isService()
{
    return metaData_["type"] == "service";
}