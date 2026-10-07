#include "paths.hpp"

#include <windows.h>
#include <filesystem>
#include <string>
#include <iostream>

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

std::filesystem::path Paths::root() const
{
    return root_;
}

std::filesystem::path Paths::bin() const
{
    return root_ / "bin";
}

std::filesystem::path Paths::runner() const
{
#if defined(BUILD_DEV)
    return root_ / "build" / "runner.exe";
#else
    return root_ / "bin" / "runner.exe";
#endif
}

std::filesystem::path Paths::python() const
{
    return root_ / "bin" / "python" / "python.exe";
}

std::filesystem::path Paths::scripts() const
{
    return root_ / "scripts";
}

std::filesystem::path Paths::sdk() const
{
    return root_ / "sdk";
}

std::filesystem::path Paths::resources() const
{
    return root_ / "resources";
}

std::filesystem::path Paths::icons() const
{
    return resources() / "icons";
}

std::filesystem::path Paths::themes() const
{
    return resources() / "themes";
}