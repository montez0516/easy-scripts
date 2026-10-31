#pragma once

#include "../ipc/Pipe.h"
#include "Message.h"
#include "MessageCodec.h"
#include "MessageFramer.h"

#include <memory>
#include <functional>
#include <thread>
#include <atomic>

class IPCConnection
{
public:
    IPCConnection();
    explicit IPCConnection(std::unique_ptr<Pipe> ipcPipe);
    ~IPCConnection();
    void send(const Message &message);
    void write(std::string_view message);

    void onMessage(std::function<void(const Message &)> callback);
    void onData(std::function<void(std::string_view)> callback);
    void readLoop();
    void startReading();
    void waitForReader();

    bool waitForConnection();
    void closeRead();
    void closeWrite();
    void requestStop();

private:
    std::unique_ptr<Pipe> ipcPipe_;
    MessageFramer framer_;

    std::function<void(const Message &)> onMessageCallback_;
    std::function<void(std::string_view)> onDataCallback_;

    std::thread readLoopThread__;
    std::atomic_bool stopped{false};
    void processBytes(std::span<const std::byte> bytes);
};