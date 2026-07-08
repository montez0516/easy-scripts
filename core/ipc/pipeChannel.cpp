#include <windows.h>
#include <iostream>

#include "pipeChannel.hpp"

PipeChannel::PipeChannel()
{
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hRead, &hWrite, &saAttr, 0))
    {
        std::cerr << "Pipe create failed" << std::endl;
        return;
    }
    writeHandle = hWrite;
    readHandle = hRead;
    SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, 0);
}

PipeChannel::~PipeChannel()
{
    CloseHandle(writeHandle);
    CloseHandle(readHandle);
}

void PipeChannel::send(const Message &message)
{
    DWORD bytesWritten;
    WriteFile(writeHandle, message.data.data(), sizeof(message.data), &bytesWritten, NULL);
}

HANDLE PipeChannel::getRead() const
{
    return readHandle;
}

HANDLE PipeChannel::getWrite() const
{
    return writeHandle;
}