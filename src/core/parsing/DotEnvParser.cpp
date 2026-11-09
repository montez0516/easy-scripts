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

    envFile_.close();

    return envMap;
}

int DotEnvParser::read()
{
    return envFile_.get();
}

void DotEnvParser::unread()
{
    envFile_.unget();
}

void DotEnvParser::readWhiteSpace()
{
    int c = read();
    while (c != std::char_traits<char>::eof() && c != '\n' && std::isspace(c))
    {
        c = read();
    }

    if (c != std::char_traits<char>::eof())
        unread();
}

bool DotEnvParser::isCharAllowed(char c)
{
    return (std::isalpha(c) || std::isdigit(c) || std::ispunct(c));
}

std::string DotEnvParser::parseKey()
{
    spdlog::debug("DotEnv::parseKey");

    std::string key;
    int c;

    readWhiteSpace();

    while (c = read())
    {
        if (isCharAllowed(c))
        {
            key += c;
        }

        if (std::isspace(c))
            break;
    }

    readWhiteSpace();

    c = read();

    if (c != '=')
        return "";

    return key;
}

std::string DotEnvParser::parseValue()
{
    spdlog::debug("DotEnvParser::parseValue");

    bool enclosed = false;
    std::string value;
    int c;

    readWhiteSpace();

    c = read();

    if (c == '\'' || c == '"')
        enclosed = true;

    while (c = read())
    {

        if (isCharAllowed(c))
        {
            if ((c == '\'' || c == '"') && enclosed)
                return value;
            value += c;
        }

        if (std::isspace(c))
            return value;
    }

    return value;
}
