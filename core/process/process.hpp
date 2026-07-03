#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class Process
{
private:
    fs::path exe;
    std::vector<std::string> args;
    std::wstring buildCommandLine();

public:
    Process(const fs::path &executable, const std::vector<std::string> &arguments);
    void start();
    void stop();
};

#endif
