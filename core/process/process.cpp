#include "process.hpp"
#include "utils.hpp"

#include <windows.h>
#include <utility>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Process::Process(fs::path executable, std::vector<std::string> arguments) : exe(std::move(executable)), args(std::move(arguments))
{
}

void Process::start()
{

    startupInfo = {};
    processInformation = {};

    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;

    stdinPipe = new UnnamedPipe();
    stdoutPipe = new UnnamedPipe();

    startupInfo.hStdInput = stdinPipe->getRead();
    startupInfo.hStdOutput = stdoutPipe->getWrite();
    startupInfo.hStdError = stdoutPipe->getWrite();

    SetHandleInformation(stdinPipe->getWrite(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stdoutPipe->getRead(), HANDLE_FLAG_INHERIT, 0);

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

    stdinPipe->closeRead();
    stdoutPipe->closeWrite();
}

std::string Process::read()
{
    if (stdoutPipe == nullptr)
    {
        std::cerr << "child stdout pipe is null cannot read" << std::endl;
        return "";
    }
    std::cout << "Reading from child stdout pipe" << std::endl;
    return stdoutPipe->read();
}

void Process::write(std::string &data)
{
    if (!data.ends_with('\n'))
        data += '\n';
    stdinPipe->write(data);
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
    delete stdinPipe;
    delete stdoutPipe;
}

static std::map<std::wstring, std::wstring> getCurrentEnvironment()
{
    std::map<std::wstring, std::wstring> variables;

    LPWCH currentEnvironment = GetEnvironmentStringsW();

    if (currentEnvironment == nullptr)
    {
        std::cerr << "Could not find Environment" << std::endl;
        return variables;
    }

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
