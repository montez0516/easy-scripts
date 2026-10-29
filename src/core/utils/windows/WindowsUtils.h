#pragma once

#include <string>
#include <windows.h>

namespace Utils::Windows
{
    std::string GetError();
    std::wstring quoteWindowsArgument(const std::wstring &argument);
    std::string GetErrorFromCode(DWORD code);
}