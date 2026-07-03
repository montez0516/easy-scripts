#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <string>
#include <vector>
#include <windows.h>

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