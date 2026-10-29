#include "IPCConnection.h"
#include "MessageCodec.h"
#include "Message.h"
#include "../ipc/Pipe.h"
#include "MessageFramer.h"
#include "../utils/windows/WindowsUtils.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <string_view>
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
    if (!ipcPipe_)
        spdlog::debug("IPCConnection:: IPCPipe is nullptr ??");
};

void IPCConnection::send(const Message &message)
{
    if (!ipcPipe_)
        return;
    std::string encodedMessage = MessageCodec::encode(message) + '\n';

    WriteResult result = ipcPipe_.get()->write(std::as_bytes(std::span(encodedMessage)));

    if (result.status == WRITE_ERROR)
    {
        spdlog::error("IPCConnection::send error while writing message to pipe {}", Utils::Windows::GetErrorFromCode(result.errorCode));
    }
}

void IPCConnection::onMessage(std::function<void(const Message &)> callback)
{
    if (!onDataCallback_)
    {
        onMessageCallback_ = std::move(callback);
        startReading();
    }
}

void IPCConnection::onData(std::function<void(std::string_view)> callback)
{
    if (!onMessageCallback_)
    {
        onDataCallback_ = std::move(callback);
        startReading();
    }
}

void IPCConnection::readLoop()
{
    std::array<std::byte, 4096> buffer;

    while (true)
    {
        std::vector<std::string> messages{};

        if (stopped)
            return;

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
            spdlog::error("IPCConnection::readLoop Error while reading from pipe {}", Utils::Windows::GetErrorFromCode(result.errorCode));
            return;
        }
        }
    }
}

void IPCConnection::startReading()
{
    if (!ipcPipe_ || readLoopThread__.joinable())
        return;

    readLoopThread__ = std::thread(&IPCConnection::readLoop, this);
}

void IPCConnection::waitForReader()
{
    if (!readLoopThread__.joinable())
        return;

    readLoopThread__.join();
}

void IPCConnection::write(std::string_view message)
{
    if (!ipcPipe_)
        return;
    WriteResult result = ipcPipe_->write(std::as_bytes(std::span(message)));

    if (result.status == WRITE_ERROR)
    {
        spdlog::error("IPCConnection::write error while writing message to pipe {}", Utils::Windows::GetErrorFromCode(result.errorCode));
    }
}

bool IPCConnection::waitForConnection()
{
    if (!ipcPipe_)
        return false;
    return ipcPipe_->waitForConnection();
}

void IPCConnection::closeRead()
{
    if (!ipcPipe_)
        return;
    ipcPipe_->closeReadHandle();
}

void IPCConnection::closeWrite()
{
    if (!ipcPipe_)
        return;
    ipcPipe_->closeWriteHandle();
}

void IPCConnection::requestStop()
{
    if (!readLoopThread__.joinable())
        return;

    stopped.store(true);

    HANDLE threadHandle = reinterpret_cast<HANDLE>(readLoopThread__.native_handle());

    BOOL success = CancelSynchronousIo(threadHandle);

    if (!success)
    {
        DWORD error = GetLastError();
        if (error != ERROR_NOT_FOUND)
            spdlog::error("IPCConnection::terminate error while terminating connection readLoop thread {}", Utils::Windows::GetErrorFromCode(error));
    }

    readLoopThread__.join();
}