#include "DotEnv.h"
#include "DotEnvParser.h"
#include "../utils/windows/WindowsUtils.h"

#include <spdlog/spdlog.h>

#include <string>
#include <fstream>
#include <utility>
#include <unordered_map>
#include <limits>
#include <iostream>

DotEnv::DotEnv(std::string path) : envPath_(std::move(path)) {}

bool DotEnv::load()
{
    DotEnvParser parser(envPath_);

    envMap_ = parser.parse();

    if (envMap_.empty())
        return false;

    return true;
}

std::string DotEnv::getenv(const std::string &key)
{
    if (!envMap_.contains(key))
    {
        return "";
    }

    return envMap_.at(key);
}

void DotEnv::print()
{

    for (const auto &[key, value] : envMap_)
    {
        std::cout << key << ":" << value << std::endl;
    }
}