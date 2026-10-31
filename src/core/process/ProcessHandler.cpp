#include "ProcessHandler.h"

#include "../communication/Message.h"
#include "../communication/IPCConnection.h"
#include "../utils/Conversion.h"
#include "../utils/windows/WindowsUtils.h"
#include "../utils/windows/Environment.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <unordered_map>
#include <utility>
#include <filesystem>
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

    if (captureHandles_)
    {
        stdoutPipe_->waitForReader();
        stderrPipe_->waitForReader();
    }
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
        spdlog::error("Process(start): CreateProcessW failed {} {}", Utils::Windows::GetError(), exe_.string());
        return false;
    }

    if (captureHandles_)
    {
        stdinPipe_->closeRead();
        stdoutPipe_->closeWrite();
        stderrPipe_->closeWrite();

        if (readyReadCallBack_)
            stdoutPipe_->onMessage(readyReadCallBack_);

        stderrPipe_->onData([this](std::string_view message)
                            {
                               std::lock_guard lock(stderrMutex_);
                               stderrBuffer_.append(message); });
    }

    waitThread_ = std::thread(&Process::t_wait, this);
    return true;
}

void Process::write(const Message &message)
{
    stdinPipe_->send(message);
}

std::wstring Process::buildCommandLine()
{
    std::wstring cmd = Utils::Windows::quoteWindowsArgument(exe_.wstring());

    for (const auto &arg : args_)
    {
        cmd += L' ';
        cmd += Utils::Windows::quoteWindowsArgument(Utils::Conversion::toWstring(arg));
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
        spdlog::error("Process(t_wait): GetExitCodeProcess failed ({})", Utils::Windows::GetError());
    }

    {
        std::lock_guard lock(exitMutex_);

        exited_ = true;
    }

    CloseHandle(processInformation_.hProcess);
    CloseHandle(processInformation_.hThread);

    processInformation_.hProcess = nullptr;
    processInformation_.hThread = nullptr;

    if (captureHandles_)
    {
        stdoutPipe_->waitForReader();
        stderrPipe_->waitForReader();
    }

    exitCV_.notify_all();

    spdlog::debug("Calling finished callback");
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
        spdlog::error("Process(terminate): Failed to terminate process {}", Utils::Windows::GetError());

        return false;
    }

    return true;
}

void Process::setEnvironment(
    const std::unordered_map<std::wstring, std::wstring> &overrides)
{
    std::unordered_map<std::wstring, std::wstring> variables = Utils::Windows::Environment::getCurrent();

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

    startupInfo_.hStdInput = stdinPipe->getReadHandle();
    startupInfo_.hStdOutput = stdoutPipe->getWriteHandle();
    startupInfo_.hStdError = stderrPipe->getWriteHandle();

    SetHandleInformation(stdinPipe->getWriteHandle(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stdoutPipe->getReadHandle(), HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stderrPipe->getReadHandle(), HANDLE_FLAG_INHERIT, 0);

    stdinPipe_ = std::make_unique<IPCConnection>(std::move(stdinPipe));
    stdoutPipe_ = std::make_unique<IPCConnection>(std::move(stdoutPipe));
    stderrPipe_ = std::make_unique<IPCConnection>(std::move(stderrPipe));

    return true;
}

void Process::setCaptureHandles(bool value)
{
    captureHandles_ = value;
}