#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <string>
#include <vector>
#include <windows.h>

static std::wstring GetError()
{
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
    {
        return L"No error occurred.";
    }

    LPWSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string representation of that error ID
    size_t size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorMessageID,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&messageBuffer,
        0,
        NULL);

    // Copy the message into a std::wstring
    std::wstring message(messageBuffer, size);

    // Free the Win32 allocated buffer
    LocalFree(messageBuffer);

    // Strip trailing newlines if present
    if (!message.empty() && message.back() == L'\n')
        message.pop_back();
    if (!message.empty() && message.back() == L'\r')
        message.pop_back();

    return message;
}

inline static std::wstring toWstring(const std::string &str)
{
    if (str.empty())
        return {};

    int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        nullptr,
        0);

    std::wstring result(size, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        result.data(),
        size);

    return result;
}

inline static std::string quoteArgument(std::string str)
{
    return "\"" + str + "\"";
}

#endif