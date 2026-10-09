#pragma once

#include "../core/Paths.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <string>
#include <filesystem>

class ScriptInfo
{
public:
    ScriptInfo(std::filesystem::path scriptName);
    std::string get(std::string key);
    nlohmann::json getAll();
    bool running{false};

private:
    std::filesystem::path scriptDir_;
    nlohmann::json scriptInfo_;

    void loadMetadata();
    void findScriptFile();
    void validateInfo();
};
