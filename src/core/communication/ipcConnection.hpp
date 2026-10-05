#pragma once

#include "../ipc/pipe.hpp"
#include "message.hpp"
#include "messageCodec.hpp"
#include "messageFramer.hpp"

#include <memory>
#include <functional>

class IPCConnection
{
public:
    IPCConnection();
    explicit IPCConnection(std::unique_ptr<Pipe> ipcPipe);

    void send(const Message &message);
    std::string read();
    void write(std::string_view message);
    void onMessage(std::function<void(const Message &)> callback);
    void onData(std::function<void(std::string_view)> callback);
    bool waitForConnection();
    void closeRead();
    void closeWrite();
    void close();

private:
    std::unique_ptr<Pipe> ipcPipe_;
    MessageFramer framer_;
};