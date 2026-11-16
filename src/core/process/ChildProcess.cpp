#include "ChildProcess.h"
#include "../ipc/UnnamedPipe.h"
#include "../communication/IPCConnection.h"
#include "../communication/Message.h"

#include <windows.h>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>

static constexpr DWORD FORCED_TERMINATION = 1;

ChildProcess::~ChildProcess()
{
    if (processInformation_.hProcess)
    {
        DWORD result = WaitForSingleObject(processInformation_.hProcess, 0);

        if (result == WAIT_TIMEOUT)
        {
            terminate();

            WaitForSingleObject(processInformation_.hProcess, INFINITE);
        }

        CloseHandle(processInformation_.hProcess);
    }

    if (processInformation_.hThread)
        CloseHandle(processInformation_.hThread);
}

bool ChildProcess::start(std::wstring &cmd, std::wstring &cwd, std::vector<wchar_t> environment)
{
    if (cmd.empty())
        return false;

    startupInfo_ = {};
    processInformation_ = {};

    startupInfo_.cb = sizeof(startupInfo_);

    if (!createPipes())
        return false;

    BOOL success = CreateProcessW(
        NULL,
        cmd.data(),
        NULL,
        NULL,
        TRUE,
        CREATE_UNICODE_ENVIRONMENT,
        environment.empty() ? NULL : reinterpret_cast<LPVOID>(environment.data()),
        cwd.empty() ? NULL : cwd.c_str(),
        &startupInfo_,
        &processInformation_);

    if (!success)
        return false;

    stdinPipe_->closeRead();
    stdoutPipe_->closeWrite();
    stderrPipe_->closeWrite();

    return true;
}

bool ChildProcess::createPipes()
{
    std::unique_ptr stdinPipe = std::make_unique<UnnamedPipe>();
    std::unique_ptr stdoutPipe = std::make_unique<UnnamedPipe>();
    std::unique_ptr stderrPipe = std::make_unique<UnnamedPipe>();

    startupInfo_.dwFlags = STARTF_USESTDHANDLES;

    startupInfo_.hStdInput = stdinPipe->getReadHandle();
    startupInfo_.hStdOutput = stdoutPipe->getWriteHandle();
    startupInfo_.hStdError = stderrPipe->getWriteHandle();

    if (!SetHandleInformation(stdinPipe->getWriteHandle(), HANDLE_FLAG_INHERIT, 0))
        return false;

    if (!SetHandleInformation(stdoutPipe->getReadHandle(), HANDLE_FLAG_INHERIT, 0))
        return false;

    if (!SetHandleInformation(stderrPipe->getReadHandle(), HANDLE_FLAG_INHERIT, 0))
        return false;

    stdinPipe_ = std::make_unique<IPCConnection>(std::move(stdinPipe));
    stdoutPipe_ = std::make_unique<IPCConnection>(std::move(stdoutPipe));
    stderrPipe_ = std::make_unique<IPCConnection>(std::move(stderrPipe));

    return true;
}

bool ChildProcess::terminate()
{
    if (processInformation_.hProcess == nullptr)
        return false;

    return TerminateProcess(processInformation_.hProcess, FORCED_TERMINATION);
}

void ChildProcess::write(const Message &message)
{
    stdinPipe_->send(message);
}

DWORD ChildProcess::wait()
{
    if (processInformation_.hProcess == nullptr)
        return -1;

    WaitForSingleObject(processInformation_.hProcess, INFINITE);

    DWORD code = -1;

    GetExitCodeProcess(processInformation_.hProcess, &code);

    return code;
}

bool ChildProcess::waitFor(std::chrono::milliseconds timeout)
{
    if (processInformation_.hProcess == nullptr)
        return true;

    DWORD result = WaitForSingleObject(processInformation_.hProcess, static_cast<DWORD>(timeout.count()));

    return result == WAIT_OBJECT_0;
}