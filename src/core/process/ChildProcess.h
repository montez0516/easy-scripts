#pragma once

#include "../communication/IPCConnection.h"
#include "../communication/Message.h"

#include <windows.h>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <functional>

class ChildProcess
{
public:
    ~ChildProcess();

    bool start(std::wstring &cmd, std::wstring &cwd, std::vector<wchar_t> environment);
    bool terminate();

    DWORD wait();
    bool waitFor(std::chrono::milliseconds timeout);

    void write(const Message &message);
    bool registerStdoutMessageReader(std::function<void(const Message &)> callback);
    bool registerStderrDataReader(std::function<void(std::string_view)> callback);
    void setCapturePipes(bool value);

private:
    std::unique_ptr<IPCConnection> stdinPipe_;
    std::unique_ptr<IPCConnection> stdoutPipe_;
    std::unique_ptr<IPCConnection> stderrPipe_;

    bool capturePipes_ = false;

    STARTUPINFOW startupInfo_{};
    PROCESS_INFORMATION processInformation_{};
    bool createPipes();
};