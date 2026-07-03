#include "process.hpp"
#include "utils.hpp"

#include <windows.h>
#include <utility>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Process::Process(const fs::path &executable, const std::vector<std::string> &arguments) : exe(std::move(executable)), args(std::move(arguments))
{
}

void Process::start()
{
    STARTUPINFOW si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    std::wstring command = buildCommandLine();

    if (!CreateProcessW(NULL, command.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::cerr << "CreateProcessW failed." << GetLastError() << std::endl;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

std::wstring Process::buildCommandLine()
{
    std::wstring cmd;

    cmd += exe.wstring();

    for (const auto &arg : args)
    {
        cmd += L' ';
        cmd += toWstring(quoteArgument(arg));
    }

    return cmd;
}