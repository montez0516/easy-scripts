#pragma once

#include <string>
#include <unordered_map>

namespace Utils::Windows::Environment
{
    std::unordered_map<std::wstring, std::wstring> getCurrent();

}