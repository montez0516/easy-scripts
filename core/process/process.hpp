#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <optional>
#include <map>

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
    std::optional<std::vector<wchar_t>> environment;

public:
    Process(fs::path executable, std::vector<std::string> arguments);
    void start();
    void stop();
    std::string read();
    void wait();
    void write(std::string &);

    void setEnvironment(const std::map<std::wstring, std::wstring> &variables);
    void clearEnvironment();
};

#endif
