#include "DotEnvParser.h"
#include "../utils/windows/WindowsUtils.h"

#include <spdlog/spdlog.h>

#include <string>
#include <fstream>
#include <utility>
#include <unordered_map>

DotEnvParser::DotEnvParser(std::string path) : envPath_(std::move(path)) {}

std::unordered_map<std::string, std::string> DotEnvParser::parse()
{
    envFile_ = std::ifstream(envPath_);
    std::unordered_map<std::string, std::string> envMap;

    if (!envFile_.is_open())
    {
        spdlog::error("Failed to open env file {}", Utils::Windows::GetError());
        return envMap;
    }

    while (!eof)
    {
        std::string key = parseKey();
        std::string value = parseValue();
        if (!key.empty() && !value.empty())
            envMap.emplace(key, value);
    }

    envFile_.close();

    return envMap;
}

int DotEnvParser::read()
{
    if (eof)
        return eof;

    int c = envFile_.get();
    if (c == std::char_traits<char>::eof())
        eof = true;

    return c;
}

void DotEnvParser::unread()
{
    envFile_.unget();
}

void DotEnvParser::readWhiteSpace()
{
    int c = read();
    while (!eof && c != '\n' && std::isspace(c))
    {
        c = read();
    }

    if (!eof)
        unread();
}

bool DotEnvParser::isCharAllowed(int c)
{
    return (!std::isspace(c) && c != '\'' && c != '"' && c != '=');
}

std::string DotEnvParser::parseKey()
{
    std::string key;
    int c;

    readWhiteSpace();

    while (!eof)
    {
        c = read();

        if (std::isspace(c))
            continue;

        if (!isCharAllowed(c))
            break;

        if (isCharAllowed(c))
        {
            key += c;
        }
    }

    if (eof)
        return key;

    if (c != '=')
    {
        return "";
    }

    return key;
}

std::string DotEnvParser::parseValue()
{

    bool enclosed = false;
    std::string value;
    int c;

    readWhiteSpace();

    c = read();

    if (c == '\'' || c == '"')
        enclosed = true;
    else
        unread();

    while (!eof)
    {
        c = read();

        if (!eof && isCharAllowed(c))
        {
            if ((c == '\'' || c == '"') && enclosed)
                return value;
            value += c;
        }

        if (c == '\n')
            return value;
    }

    return value;
}
