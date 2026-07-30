#include "paths.hpp"

#include <windows.h>
#include <filesystem>
#include <string>

Paths::Paths()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    #if defined(BUILD_DEV)
        root_ = std::filesystem::path(path).parent_path().parent_path();
    #else
        root_ = std::filesystem::path(path);

        if(root_.stem() == "EasyScripts")
        {
            root_ = root_.parent_path();
        }
        else if(root_.stem() == "runner")
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
    return root_ / "bin" / "runner.exe";
}

std::filesystem::path Paths::python() const
{
    return root_ / "bin" / "python" / "python.exe";
}

std::filesystem::path Paths::scripts() const
{
    return root_ / "scripts";
}
