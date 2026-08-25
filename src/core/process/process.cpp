#include "process.hpp"
#include "utils.hpp"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <utility>
#include <filesystem>
#include <iostream>
#include <thread>
#include <functional>

namespace fs = std::filesystem;

Process::Process(fs::path executable, std::vector<std::string> arguments) : exe_(std::move(executable)), args_(std::move(arguments))
{
}

Process::~Process()
{
    if (waitThread_.joinable())
        waitThread_.join();
    stdinPipe_.close();
    stdoutPipe_.close();
    stderrPipe_.close();
}

bool Process::start()
{

    startupInfo_ = {};
    processInformation_ = {};

    startupInfo_.cb = sizeof(startupInfo_);

    if (captureHandles_)
    {
        if (!captureProcessHandles())
            return false;
    }

    std::wstring command = buildCommandLine();

    LPVOID environmentBlock = nullptr;

    if (processEnvironment_.has_value())
    {
        environmentBlock = processEnvironment_->data();
    }

    if (!CreateProcessW(NULL,
                        command.data(),
                        NULL,
                        NULL,
                        TRUE,
                        CREATE_UNICODE_ENVIRONMENT,
                        environmentBlock,
                        cwd_.empty() ? NULL : cwd_.c_str(),
                        &startupInfo_,
                        &processInformation_))
    {
        spdlog::error("Process(start): CreateProcessW failed {} {}", toString(GetError()), exe_.string());
        return false;
    }

    waitThread_ = std::thread(&Process::t_wait, this);
    return true;
}

std::string Process::read()
{
    return stdoutPipe_.read();
}

void Process::write(std::string &data)
{
    if (!data.ends_with('\n'))
        data += '\n';
    stdinPipe_.write(data);
}

std::wstring Process::buildCommandLine()
{
    std::wstring cmd = quoteWindowsArgument(exe_.wstring());

    for (const auto &arg : args_)
    {
        cmd += L' ';
        cmd += quoteWindowsArgument(toWstring(arg));
    }

    return cmd;
}

void Process::t_wait()
{
    if (processInformation_.hProcess == nullptr)
    {
        throw std::runtime_error("hProcess handle null");
    }

    WaitForSingleObject(processInformation_.hProcess, INFINITE);

    if (!GetExitCodeProcess(processInformation_.hProcess, &exitCode_))
    {
        spdlog::error("Process(t_wait): GetExitCodeProcess failed ({})", toString(GetError()));
    }

    CloseHandle(processInformation_.hProcess);
    CloseHandle(processInformation_.hThread);

    processInformation_.hProcess = nullptr;
    processInformation_.hThread = nullptr;

    if (finishCallBack_)
        finishCallBack_(exitCode_);
}

DWORD Process::wait()
{
    if (waitThread_.joinable())
        waitThread_.join();
    return exitCode_;
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
    processEnvironment_ = std::move(block);
}

void Process::clearEnvironment()
{
    processEnvironment_.reset();
}

void Process::setCurrentDirectory(const std::wstring &dir)
{
    cwd_ = dir;
}

void Process::clearCurrentDirectory()
{
    cwd_.clear();
}

std::string Process::error()
{
    return stderrPipe_.read();
}

bool Process::readyRead(std::function<void()> readCallBack)
{
    return stdoutPipe_.readyRead(std::move(readCallBack));
}

void Process::onFinished(std::function<void(DWORD)> finishCallBack)
{
    finishCallBack_ = std::move(finishCallBack);
}

bool Process::captureProcessHandles()
{
    if (stdinPipe_.isNull() || stdoutPipe_.isNull() || stderrPipe_.isNull())
    {
        spdlog::debug("Process(start):\nstdin:{}\nstdout:{}\nstderr:{}\n", stdinPipe_.isNull(), stdoutPipe_.isNull(), stderrPipe_.isNull());
        spdlog::critical("Process(start): one or more ipc pipe is NULL");
        return false;
    }
    startupInfo_.dwFlags = STARTF_USESTDHANDLES;

    startupInfo_.hStdInput = stdinPipe_.getRead();
    startupInfo_.hStdOutput = stdoutPipe_.getWrite();
    startupInfo_.hStdError = stderrPipe_.getWrite();

    SetHandleInformation(stdinPipe_.getWrite(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stdoutPipe_.getRead(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stderrPipe_.getRead(), HANDLE_FLAG_INHERIT, 0);

    stdinPipe_.closeRead();
    stdoutPipe_.closeWrite();
    stderrPipe_.closeWrite();

    return true;
}

void Process::setCaptureHandles(bool value)
{
    captureHandles_ = value;
}