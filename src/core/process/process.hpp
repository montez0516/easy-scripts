#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <optional>
#include <map>

#include "../ipc/unnamedPipe.hpp"

namespace fs = std::filesystem;

class Process
{
private:
    fs::path exe;
    std::vector<std::string> args;
    std::wstring buildCommandLine();

    UnnamedPipe *stdinPipe = nullptr;
    UnnamedPipe *stdoutPipe = nullptr;
    UnnamedPipe *stderrPipe = nullptr;

    STARTUPINFOW startupInfo{};
    PROCESS_INFORMATION processInformation{};
    std::optional<std::vector<wchar_t>> environment;
    std::wstring cwd;

public:
    Process(fs::path executable, std::vector<std::string> arguments);
    void start();
    void stop();
    DWORD wait();
    std::string read();
    void write(std::string &);
    std::string error();

    void setEnvironment(const std::map<std::wstring, std::wstring> &variables);
    void clearEnvironment();

    void setCurrentDirectory(const std::wstring &dir);
    void clearCurrentDirectory();
};

#endif
