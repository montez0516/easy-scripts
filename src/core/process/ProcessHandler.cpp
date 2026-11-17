#include "ProcessHandler.h"

#include "ChildProcess.h"
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

namespace fs = std::filesystem;

ProcessHandler::ProcessHandler(fs::path executable, std::vector<std::string> arguments) : exe_(std::move(executable)), args_(std::move(arguments))
{
}

ProcessHandler::~ProcessHandler()
{
    if (childProcess_)
    {
        if (!childProcess_->waitFor(std::chrono::milliseconds(3000)))
            childProcess_->terminate();
    }

    if (waitThread_.joinable())
        waitThread_.join();
}

bool ProcessHandler::start()
{
    childProcess_ = std::make_unique<ChildProcess>();

    if (!childProcess_)
        return false;

    std::wstring commandLine = buildCommandLine();

    childProcess_->setCapturePipes(capturePipes_);

    if (!childProcess_->start(commandLine, cwd_, processEnvironment_))
        return false;

    if (stdoutCallback_)
        childProcess_->registerStdoutMessageReader(stdoutCallback_);

    if (stderrCallback_)
        childProcess_->registerStderrDataReader(stderrCallback_);

    waitThread_ = std::thread(&ProcessHandler::t_wait, this);
    return true;
}

void ProcessHandler::write(const Message &message)
{
    if (childProcess_)
        childProcess_->write(message);
}

std::wstring ProcessHandler::buildCommandLine()
{
    std::wstring cmd = Utils::Windows::quoteWindowsArgument(exe_.wstring());

    for (const auto &arg : args_)
    {
        cmd += L' ';
        cmd += Utils::Windows::quoteWindowsArgument(Utils::Conversion::toWstring(arg));
    }

    return cmd;
}

void ProcessHandler::t_wait()
{
    if (!childProcess_)
        return;
    DWORD code = childProcess_->wait();

    if (finishCallBack_)
        finishCallBack_(code);
}

DWORD ProcessHandler::wait()
{
    if (!childProcess_)
        return -1;

    return childProcess_->wait();
}

bool ProcessHandler::waitFor(std::chrono::milliseconds timeout)
{
    if (!childProcess_)
        return true;

    return childProcess_->waitFor(timeout);
}

bool ProcessHandler::terminate()
{
    if (!childProcess_)
        return true;

    return childProcess_->terminate();
}

bool ProcessHandler::shutdown()
{
    if (!childProcess_)
        return true;

    childProcess_->write({.type = "scripts.shutdown"});

    return childProcess_->waitFor(std::chrono::milliseconds(3000));
}

void ProcessHandler::setEnvironment(
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

void ProcessHandler::clearEnvironment()
{
    processEnvironment_.clear();
}

void ProcessHandler::setCurrentDirectory(const std::wstring &dir)
{
    cwd_ = dir;
}

void ProcessHandler::clearCurrentDirectory()
{
    cwd_.clear();
}

std::string ProcessHandler::error()
{
    return stderrBuffer_;
}

void ProcessHandler::registerStdoutMessageReader(std::function<void(const Message &)> callback)
{
    if (stdoutCallback_)
        return;

    stdoutCallback_ = std::move(callback);
}

void ProcessHandler::registerStderrDataReader(std::function<void(std::string_view)> callback)
{
    if (stderrCallback_)
        return;

    stderrCallback_ = std::move(callback);
}

void ProcessHandler::registerOnFinishedCallback(std::function<void(DWORD)> callback)
{
    if (finishCallBack_)
        return;

    finishCallBack_ = std::move(callback);
}

void ProcessHandler::setCapturePipes(bool value)
{
    capturePipes_ = value;
}