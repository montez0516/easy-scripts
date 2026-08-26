#ifndef SCRIPT_H
#define SCRIPT_H

#include "../core/eventBus/eventBus.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <vector>
#include <string>


class ScriptManager;

class Script
{
private:
    std::filesystem::path dir_;
    std::filesystem::path scriptFile_;
    std::filesystem::path icon_;
    nlohmann::json metaData_;

    ScriptManager &manager_;
    EventBus &bus_;
    void parseInfo();
    void loadMetaData();
    void findScriptFile();
    void logInfo();

public:
    Script(std::filesystem::path scriptDir, ScriptManager &m, EventBus &bus);
    void initialize();
    void run(std::vector<std::string> args);
    void stop();
    bool isService();
};
#endif