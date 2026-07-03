#include "pythonRuntime.hpp"

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

const fs::path PYTHON = fs::path("bin") / "python" / "python.exe";

void PythonRuntime::run(const fs::path &file, const std::vector<std::string> &args)
{
}