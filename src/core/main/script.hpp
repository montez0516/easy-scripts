#ifndef SCRIPT_H
#define SCRIPT_H

#include <filesystem>
#include <vector>
#include <string>

#include "nlohmann/json.hpp"

typedef struct scriptinfo
{
    std::filesystem::path scriptFile;
    std::filesystem::path icon;
    nlohmann::json metaData;
} ScriptInfo;

class ScriptManager;

class Script
{
private:
    std::filesystem::path dir;

    ScriptInfo info;
    void parseInfo();

    ScriptManager *manager = nullptr;

public:
    Script(std::filesystem::path scriptDir, ScriptManager *m);
    void initialize();
    void run(std::vector<std::string> args);
    void stop();
};
#endif