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
#include <array>
#include <span>
#include <cstddef>

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
    ipcPipe_.get()->write(std::as_bytes(std::span(encodedMessage)));
}

void IPCConnection::onMessage(std::function<void(const Message &)> callback)
{
    if (!onDataCallback_)
        onMessageCallback_ = std::move(callback);
}

void IPCConnection::onData(std::function<void(std::string_view)> callback)
{
    if (!onMessageCallback_)
        onDataCallback_ = std::move(callback);
}

void IPCConnection::readLoop()
{
    std::array<std::byte, 4096> buffer;

    while (true)
    {
        std::vector<std::string> messages{};

        ReadResult result = ipcPipe_->read(buffer);

        switch (result.status)
        {
        case READ_SUCCESS:
        {
            std::string_view data(reinterpret_cast<const char *>(buffer.data()), result.bytesRead);

            if (onDataCallback_)
                onDataCallback_(data);

            else if (onMessageCallback_)
            {
                messages = framer_.push(data);
                for (const auto &message : messages)
                {
                    Message decodedMessage = MessageCodec::decode(message);

                    if (onMessageCallback_)
                        onMessageCallback_(decodedMessage);
                }
            }
            break;
        }
        case READ_EOF:
        {
            return;
        }
        case READ_ERROR:
        {
            spdlog::error("Error while reading from pipe {}", result.errorCode);
            return;
        }
        }
    }
}

void IPCConnection::write(std::string_view message)
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_->write(std::as_bytes(std::span(message)));
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
    ipcPipe_->closeReadHandle();
}

void IPCConnection::closeWrite()
{
    if (ipcPipe_.get() == nullptr)
        return;
    ipcPipe_->closeWriteHandle();
}
