#pragma once

#include <string>

namespace Utils::Conversion
{
    std::wstring toWstring(const std::string &str);
    std::string toString(const std::wstring &wstr);
}