#include "process.hpp"
#include "utils.hpp"
#include "../ipc/unnamedPipeChannel.hpp"

#include <windows.h>
#include <utility>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

static std::wstring GetError()
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

Process::Process(fs::path executable, std::vector<std::string> arguments) : exe(std::move(executable)), args(std::move(arguments))
{
}

void Process::start()
{
    pipe = new UnnamedPipeChannel();

    startupInfo = {};
    processInformation = {};

    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdInput = pipe->stdinPipe->getRead();
    startupInfo.hStdOutput = pipe->stdoutPipe->getWrite();
    startupInfo.hStdError = pipe->stdoutPipe->getWrite();

    SetHandleInformation(pipe->stdinPipe->getWrite(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(pipe->stdoutPipe->getRead(), HANDLE_FLAG_INHERIT, 0);

    std::wstring command = buildCommandLine();

    LPVOID environmentBlock = nullptr;

    if (environment.has_value())
    {
        environmentBlock = environment->data();
    }

    std::wcout << "Running Command: " << command << std::endl;

    if (!CreateProcessW(NULL, command.data(), NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT, environmentBlock, NULL, &startupInfo, &processInformation))
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

static std::map<std::wstring, std::wstring> getCurrentEnvironment()
{
    std::map<std::wstring, std::wstring> variables;

    LPWCH currentEnvironment = GetEnvironmentStringsW();

    if (currentEnvironment == nullptr)
        return variables;

    for (const wchar_t *current = currentEnvironment; *current != L'\0'; current += std::wcslen(current) + 1)
    {
        std::wstring entry(current);

        std::size_t separator = entry[0] == L'=' ? entry.find(L'=', 1) : entry.find(L'=');

        if (separator != std::wstring::npos)
        {
            variables.emplace(entry.substr(0, separator), entry.substr(separator + 1));
        }
    }

    FreeEnvironmentStringsW(currentEnvironment);
    return variables;
}

void Process::setEnvironment(
    const std::map<std::wstring, std::wstring> &overrides)
{
    std::map<std::wstring, std::wstring> variables = getCurrentEnvironment();

    for (const auto &[name, value] : overrides)
        variables[name] = value;

    std::vector<wchar_t> block;

    for (const auto &[name, value] : variables)
    {
        const std::wstring entry = name + L"=" + value;

        block.insert(block.end(), entry.begin(), entry.end());
        block.push_back(L'\0');
    }

    block.push_back(L'\0');
    environment = std::move(block);
}

void Process::clearEnvironment()
{
    environment.reset();
}
