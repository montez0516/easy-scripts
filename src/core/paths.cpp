#include "Paths.h"

#include <windows.h>
#include <filesystem>
#include <string>
#include <iostream>

std::filesystem::path Paths::root_;

Paths::Paths()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

#if defined(BUILD_DEV)
    root_ = std::filesystem::path(path).parent_path().parent_path();
#else
    root_ = std::filesystem::path(path);

    if (root_.stem() == "EasyScripts")
    {
        root_ = root_.parent_path();
    }
    else if (root_.stem() == "runner")
    {
        root_ = root_.parent_path().parent_path();
    }

#endif
}

std::filesystem::path Paths::root()
{
    return root_;
}

std::filesystem::path Paths::bin()
{
    return root_ / "bin";
}

std::filesystem::path Paths::runner()
{
#if defined(BUILD_DEV)
    return root_ / "build" / "runner.exe";
#else
    return root_ / "bin" / "runner.exe";
#endif
}

std::filesystem::path Paths::python()
{
    return root_ / "bin" / "python" / "python.exe";
}

std::filesystem::path Paths::scripts()
{
    return root_ / "scripts";
}

std::filesystem::path Paths::sdk()
{
    return root_ / "sdk";
}

std::filesystem::path Paths::resources()
{
    return root_ / "resources";
}

std::filesystem::path Paths::icons()
{
    return resources() / "icons";
}

std::filesystem::path Paths::themes()
{
    return resources() / "themes";
}