#include <filesystem>
#include <string>
#include "core/runner/runtime/runtimeManager.hpp"
#include "core/runner/engine/runnerEngine.h"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    Engine *engine = new Engine;
    engine->initialize();

    std::string language;
    std::string file;
    std::string args;

    if (argc < 3)
        return 1;

    language = argv[1];
    file = argv[2];

    if (argc >= 4)
        args = argv[3];

    engine->run(language, file, args);
    return 0;
}