#ifndef SCRIPT_H
#define SCRIPT_H

#include <filesystem>
#include <vector>
#include <string>

#include "nlohmann/json.hpp"

class ScriptManager;

class Script
{
private:
    std::filesystem::path dir;

    std::filesystem::path scriptFile;
    std::filesystem::path icon;
    nlohmann::json metaData;

    ScriptManager *manager = nullptr;

    void parseInfo();
    void loadMetaData();
    void findScriptFile();
    void logInfo();

public:
    Script(std::filesystem::path scriptDir, ScriptManager *m);
    void initialize();
    void run(std::vector<std::string> args);
    void stop();
    bool isService();
};
#endif