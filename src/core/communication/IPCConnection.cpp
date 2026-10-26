#include "IPCConnection.h"
#include "MessageCodec.h"
#include "Message.h"
#include "../ipc/Pipe.h"
#include "MessageFramer.h"

#include <spdlog/spdlog.h>

#include <string>
#include <memory>
#include <functional>
#include <utility>
#include <vector>

IPCConnection::IPCConnection() {};

IPCConnection::IPCConnection(std::unique_ptr<Pipe> ipcPipe) : ipcPipe_(std::move(ipcPipe))
{
    if (ipcPipe_.get() == nullptr)
        spdlog::debug("IPCConnection:: IPCPipe is nullptr ??");
};

void IPCConnection::send(const Message &message)
{
    if (ipcPipe_.get() == nullptr)
        return;
    std::string encodedMessage = MessageCodec::encode(message) + '\n';
    ipcPipe_.get()->write(encodedMessage);
}

void IPCConnection::onMessage(std::function<void(const Message &)> callback)
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_.get()->readyRead([this, callback](std::string_view message)
                              {
                                  std::vector<std::string> messages = framer_.push(message);

                                  for (const std::string &encodedMessage : messages)
                                  {
                                      Message decodedMessage = MessageCodec::decode(encodedMessage);
                                      callback(decodedMessage);
                                  } });
}

void IPCConnection::onData(std::function<void(std::string_view)> callback)
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_->readyRead(std::move(callback));
}

std::string IPCConnection::read()
{
    if (ipcPipe_.get() == nullptr)
        return "";
    return ipcPipe_->read();
}

void IPCConnection::write(std::string_view message)
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_->write(message);
}

bool IPCConnection::waitForConnection()
{
    if (ipcPipe_.get() == nullptr)
        return false;
    return ipcPipe_->waitForConnection();
}

void IPCConnection::closeRead()
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_->closeRead();
}

void IPCConnection::closeWrite()
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_->closeWrite();
}

void IPCConnection::close()
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_->close();
}