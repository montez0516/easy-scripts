#include "Environment.h"
#include "WindowsUtils.h"
#include "../Conversion.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <string>
#include <unordered_map>

namespace Utils::Windows::Environment
{
    std::unordered_map<std::wstring, std::wstring> getCurrent()
    {
        std::unordered_map<std::wstring, std::wstring> variables;

        LPWCH currentEnvironment = GetEnvironmentStringsW();

        if (currentEnvironment == nullptr)
        {
            spdlog::error("Utils::Windows::Environment::getCurrent Could not find Environment {}", Utils::Windows::GetError());
            return variables;
        }

        for (const wchar_t *current = currentEnvironment; *current != L'\0'; current += std::wcslen(current) + 1)
        {
            std::wstring entry(current);

            std::size_t separator = entry[0] == L'=' ? entry.find(L'=', 1) : entry.find(L'=');

            if (separator != std::wstring::npos)
            {
                variables.emplace(entry.substr(0, separator), entry.substr(separator + 1));
            }
        }

        FreeEnvironmentStringsW(currentEnvironment);
        return variables;
    }
}