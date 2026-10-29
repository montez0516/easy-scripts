#include "Conversion.h"

#include <windows.h>
#include <string>

namespace Utils::Conversion
{
    std::wstring toWstring(const std::string &str)
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
            return {};
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
            return {};
        }

        return result;
    }

    std::string toString(const std::wstring &wstr)
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
}