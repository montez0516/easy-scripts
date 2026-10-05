#include "process.hpp"
#include "utils.hpp"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <utility>
#include <filesystem>
#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

static constexpr DWORD FORCED_TERMINATION = 1;

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
                        captureHandles_ ? TRUE : FALSE,
                        CREATE_UNICODE_ENVIRONMENT,
                        environmentBlock,
                        cwd_.empty() ? NULL : cwd_.c_str(),
                        &startupInfo_,
                        &processInformation_))
    {
        spdlog::error("Process(start): CreateProcessW failed {} {}", toString(GetError()), exe_.string());
        return false;
    }

    if (captureHandles_)
    {
        stdinPipe_.closeRead();
        stdoutPipe_.closeWrite();
        stderrPipe_.closeWrite();

        if (readyReadCallBack_)
            stdoutPipe_.onMessage(readyReadCallBack_);

        stderrPipe_.onData([this](std::string_view message)
                           {
                               std::lock_guard lock(stderrMutex_);
                               stderrBuffer_.append(message); });
    }

    waitThread_ = std::thread(&Process::t_wait, this);
    return true;
}

std::string Process::read()
{
    return stdoutPipe_.read();
}

void Process::write(const Message &message)
{
    stdinPipe_.send(message);
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
        spdlog::error("Process(t_wait): hProcess handle null");
        return;
    }

    WaitForSingleObject(processInformation_.hProcess, INFINITE);

    if (!GetExitCodeProcess(processInformation_.hProcess, &exitCode_))
    {
        spdlog::error("Process(t_wait): GetExitCodeProcess failed ({})", toString(GetError()));
    }

    {
        std::lock_guard lock(exitMutex_);

        exited_ = true;
    }

    CloseHandle(processInformation_.hProcess);
    CloseHandle(processInformation_.hThread);

    processInformation_.hProcess = nullptr;
    processInformation_.hThread = nullptr;

    stdinPipe_.close();
    stdoutPipe_.close();
    stderrPipe_.close();

    exitCV_.notify_all();

    if (finishCallBack_)
        finishCallBack_(exitCode_);
}

DWORD Process::wait()
{
    if (waitThread_.joinable())
        waitThread_.join();
    return exitCode_;
}

bool Process::waitFor(std::chrono::milliseconds timeout)
{
    if (processInformation_.hProcess == nullptr)
        return true;

    std::unique_lock lock(exitMutex_);

    return exitCV_.wait_for(lock, timeout, [this]()
                            { return exited_; });
}

bool Process::terminate()
{
    if (processInformation_.hProcess == nullptr)
    {
        return false;
    }

    if (!TerminateProcess(processInformation_.hProcess, FORCED_TERMINATION))
    {
        spdlog::error("Process(terminate): Failed to terminate process {}", toString(GetError()));

        return false;
    }

    return true;
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
    return stderrBuffer_;
}

void Process::registerReadyReadCallback(std::function<void(const Message &)> callback)
{
    readyReadCallBack_ = std::move(callback);
}

void Process::registerOnFinishedCallback(std::function<void(DWORD)> callback)
{
    finishCallBack_ = std::move(callback);
}

bool Process::captureProcessHandles()
{
    std::unique_ptr stdinPipe = std::make_unique<UnnamedPipe>();
    std::unique_ptr stdoutPipe = std::make_unique<UnnamedPipe>();
    std::unique_ptr stderrPipe = std::make_unique<UnnamedPipe>();

    startupInfo_.dwFlags = STARTF_USESTDHANDLES;

    startupInfo_.hStdInput = stdinPipe->getRead();
    startupInfo_.hStdOutput = stdoutPipe->getWrite();
    startupInfo_.hStdError = stderrPipe->getWrite();

    SetHandleInformation(stdinPipe->getWrite(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stdoutPipe->getRead(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stderrPipe->getRead(), HANDLE_FLAG_INHERIT, 0);

    stdinPipe_ = IPCConnection(std::move(stdinPipe));
    stdoutPipe_ = IPCConnection(std::move(stdoutPipe));
    stderrPipe_ = IPCConnection(std::move(stderrPipe));

    return true;
}

void Process::setCaptureHandles(bool value)
{
    captureHandles_ = value;
}