#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <fstream>

class DotEnv
{
public:
    explicit DotEnv(std::string path);
    bool load();
    std::string getenv(const std::string &key);
    void print();

private:
    std::string envPath_;
    std::unordered_map<std::string, std::string> envMap_;
};