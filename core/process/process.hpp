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
    UnnamedPipeChannel *pipe = nullptr;

    STARTUPINFOW startupInfo{};
    PROCESS_INFORMATION processInformation{};

public:
    Process(const fs::path &executable, const std::vector<std::string> &arguments);
    void start();
    void stop();
    std::string read();
    void wait();
    void write(std::string &);
};

#endif
