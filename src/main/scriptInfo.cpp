#include "scriptInfo.hpp"
#include "../core/paths.hpp"

#include <nlohmann/json.hpp>

#include <string>
#include <filesystem>
#include <fstream>

ScriptInfo::ScriptInfo(std::filesystem::path scriptDir) : scriptDir_(scriptDir)
{
    loadMetadata();
    findScriptFile();
    scriptInfo_["dir"] = scriptDir.string();
}

void ScriptInfo::loadMetadata()
{
    std::filesystem::path metaPath = scriptDir_ / "meta.json";

    if (!std::filesystem::exists(metaPath))
    {
        scriptInfo_["name"] = scriptDir_.stem();
        return;
    }

    std::ifstream metaFile(metaPath);
    scriptInfo_ = nlohmann::json::parse(metaFile);
    metaFile.close();
}

void ScriptInfo::findScriptFile()
{
    std::vector<std::string> script_extensions = {".py", ".exe"};

    for (const auto &ext : script_extensions)
    {
        std::filesystem::path filename = scriptDir_ / ("main" + ext);
        if (std::filesystem::exists(filename))
            scriptInfo_["file"] = filename.string();
    }
}

std::string ScriptInfo::get(std::string key)
{
    return scriptInfo_.value<std::string>(key, "");
}

nlohmann::json ScriptInfo::getAll()
{
    return scriptInfo_;
}