#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

class DotEnvParser
{
public:
    explicit DotEnvParser(std::string path);
    std::unordered_map<std::string, std::string> parse();

private:
    int read();
    void unread();
    void readWhiteSpace();

    std::string parseKey();
    std::string parseValue();
    bool isCharAllowed(char c);

    std::ifstream envFile_;
    std::string envPath_;
};
