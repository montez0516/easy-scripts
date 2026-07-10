#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <filesystem>

#include "../ipc/unnamedPipeChannel.hpp"

namespace fs = std::filesystem;

class Process
{
private:
    fs::path exe;
    std::vector<std::string> args;
    std::wstring buildCommandLine();
    UnnamedPipeChannel *pipe;

public:
    Process(const fs::path &executable, const std::vector<std::string> &arguments);
    void start();
    void stop();
};

#endif
