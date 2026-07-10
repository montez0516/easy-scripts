#include <windows.h>
#include <string>
#include <iostream>
#include "unnamedPipe.hpp"

UnnamedPipe::UnnamedPipe()
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

UnnamedPipe::~UnnamedPipe()
{
    CloseHandle(readHandle);
    CloseHandle(writeHandle);
}

std::string UnnamedPipe::read()
{
    if (readHandle == nullptr)
        return "";

    std::string output;

    char buffer[4096];
    DWORD bytesRead = 0;

    if (!ReadFile(readHandle, buffer, sizeof(buffer), &bytesRead, NULL))
        return "";

    output.append(buffer, bytesRead);

    return output;
}

void UnnamedPipe::write(const std::string &data)
{
    if (writeHandle == nullptr || data.empty())
        return;

    DWORD bytesWritten = 0;

    WriteFile(writeHandle, data.data(), data.size(), &bytesWritten, NULL);
}

HANDLE UnnamedPipe::getRead() const
{
    return readHandle;
}

HANDLE UnnamedPipe::getWrite() const
{
    return writeHandle;
}