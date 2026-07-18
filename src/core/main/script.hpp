#ifndef SCRIPT_H
#define SCRIPT_H

#include <filesystem>
#include "nlohmann/json.hpp"

typedef struct scriptinfo
{
    std::filesystem::path scriptFile;
    std::filesystem::path icon;
    nlohmann::json metaData;
    bool hasEnv;

} ScriptInfo;

class Script
{
private:
    std::filesystem::path dir;

    ScriptInfo info;
    void parseInfo();

public:
    Script(std::filesystem::path scriptDir);
};
#endif