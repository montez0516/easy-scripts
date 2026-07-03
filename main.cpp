#include "core/process/process.hpp"
#include <filesystem>

namespace fs = std::filesystem;

int main()
{

    fs::path python = fs::path("bin") / "python" / "python.exe";

    Process process(python, {"main.py"});
    process.start();
    return 0;
}