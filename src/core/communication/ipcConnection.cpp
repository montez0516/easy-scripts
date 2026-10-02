#include "ipcConnection.hpp"
#include "messageCodec.hpp"
#include "message.hpp"
#include "../ipc/pipe.hpp"

#include <string>
#include <memory>
#include <functional>
#include <utility>

IPCConnection::IPCConnection() {};

IPCConnection::IPCConnection(std::unique_ptr<Pipe> ipcPipe) : ipcPipe_(std::move(ipcPipe)) {};

void IPCConnection::send(const Message &message)
{
    std::string encodedMessage = MessageCodec::encode(message);
    ipcPipe_.get()->write(encodedMessage);
}

void IPCConnection::onMessage(std::function<void(const Message &)> callback)
{
    ipcPipe_.get()->readyRead([callback](std::string_view message)
                              { Message decodedMessage = MessageCodec::decode(message);
                                callback(decodedMessage); });
}

std::string IPCConnection::read()
{
    return ipcPipe_->read();
}

void IPCConnection::write(std::string_view message)
{
    ipcPipe_->write(message);
}

bool IPCConnection::waitForConnection()
{
    return ipcPipe_->waitForConnection();
}

void IPCConnection::closeRead()
{
    ipcPipe_->closeRead();
}

void IPCConnection::closeWrite()
{
    ipcPipe_->closeWrite();
}

void IPCConnection::close()
{
    ipcPipe_->close();
}