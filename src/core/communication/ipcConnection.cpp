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

IPCConnection::IPCConnection(std::unique_ptr<Pipe> ipcPipe) : ipcPipe_(std::move(ipcPipe)) {};

void IPCConnection::send(const Message &message)
{
    std::string encodedMessage = MessageCodec::encode(message) + '\n';
    ipcPipe_.get()->write(encodedMessage);
}

void IPCConnection::onMessage(std::function<void(const Message &)> callback)
{
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
    ipcPipe_->readyRead(std::move(callback));
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