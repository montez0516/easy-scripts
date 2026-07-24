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

    const int requiredSize = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        str.data(),
        static_cast<int>(str.size()),
        nullptr,
        0);

    if (requiredSize == 0)
    {
        throw std::runtime_error(
            "MultiByteToWideChar size calculation failed");
    }

    std::wstring result(
        static_cast<std::size_t>(requiredSize),
        L'\0');

    const int convertedSize = MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        str.data(),
        static_cast<int>(str.size()),
        result.data(),
        requiredSize);

    if (convertedSize == 0)
    {
        throw std::runtime_error(
            "MultiByteToWideChar conversion failed");
    }

    return result;
}

inline std::string toString(const std::wstring &wstr)
{
    if (wstr.empty())
        return {};

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    if (sizeNeeded == 0)
        return {};

    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), str.data(), sizeNeeded, nullptr, nullptr);

    return str;
}

static std::wstring quoteWindowsArgument(const std::wstring &argument)
{
    if (argument.empty())
        return L"\"\"";

    const bool requiresQuotes =
        argument.find_first_of(L" \t\n\v\"") != std::wstring::npos;

    if (!requiresQuotes)
        return argument;

    std::wstring result = L"\"";
    std::size_t backslashes = 0;

    for (wchar_t character : argument)
    {
        if (character == L'\\')
        {
            ++backslashes;
            continue;
        }

        if (character == L'"')
        {
            // Backslashes before a quote must be doubled,
            // and the quote itself must be escaped.
            result.append(backslashes * 2 + 1, L'\\');
            result.push_back(L'"');
            backslashes = 0;
            continue;
        }

        result.append(backslashes, L'\\');
        backslashes = 0;
        result.push_back(character);
    }

    // Backslashes before the closing quote must be doubled.
    result.append(backslashes * 2, L'\\');
    result.push_back(L'"');

    return result;
}

#endif