#include "process.hpp"
#include "utils.hpp"
#include "../ipc/unnamedPipeChannel.hpp"

#include <windows.h>
#include <utility>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::wstring GetError()
{
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
    {
        return L"No error occurred.";
    }

    LPWSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string representation of that error ID
    size_t size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorMessageID,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&messageBuffer,
        0,
        NULL);

    // Copy the message into a std::wstring
    std::wstring message(messageBuffer, size);

    // Free the Win32 allocated buffer
    LocalFree(messageBuffer);

    // Strip trailing newlines if present
    if (!message.empty() && message.back() == L'\n')
        message.pop_back();
    if (!message.empty() && message.back() == L'\r')
        message.pop_back();

    return message;
}

Process::Process(const fs::path &executable, const std::vector<std::string> &arguments) : exe(std::move(executable)), args(std::move(arguments))
{
}

void Process::start()
{
    pipe = new UnnamedPipeChannel();

    STARTUPINFOW si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);
    si.hStdInput = pipe->stdinPipe->getRead();
    si.hStdOutput = pipe->stdoutPipe->getWrite();

    std::wstring command = buildCommandLine();

    std::wcout << "Running Command: " << command << std::endl;

    if (!CreateProcessW(NULL, command.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::wcerr << "CreateProcessW failed." << GetError() << std::endl;
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