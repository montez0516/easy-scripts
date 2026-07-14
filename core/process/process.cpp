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

    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = pipe->stdinPipe->getRead();
    startupInfo.hStdOutput = pipe->stdoutPipe->getWrite();
    startupInfo.hStdError = pipe->stdoutPipe->getWrite();

    SetHandleInformation(pipe->stdinPipe->getWrite(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(pipe->stdoutPipe->getRead(), HANDLE_FLAG_INHERIT, 0);

    std::wstring command = buildCommandLine();

    std::wcout << "Running Command: " << command << std::endl;

    if (!CreateProcessW(NULL, command.data(), NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInformation))
    {
        std::wcerr << "CreateProcessW failed." << GetError() << std::endl;
    }

    pipe->stdinPipe->closeRead();
    pipe->stdoutPipe->closeWrite();
}

std::string Process::read()
{
    return pipe->stdoutPipe->read();
}

void Process::write(std::string &data)
{
    if (!data.ends_with('\n'))
        data += '\n';
    pipe->stdinPipe->write(data);
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

void Process::wait()
{
    WaitForSingleObject(processInformation.hProcess, INFINITE);

    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);

    processInformation.hProcess = nullptr;
    processInformation.hThread = nullptr;
}
