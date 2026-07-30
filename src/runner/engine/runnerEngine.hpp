#ifndef RUNNER_ENGINE_H
#define RUNNER_ENGINE_H

#include "../runtime/runtimeManager.hpp"
#include "../../core/paths.hpp"

#include <vector>
#include <string>

class Engine
{

private:
    RuntimeManager *runtimeManager = new RuntimeManager();
    Paths &paths_;
public:
    Engine(Paths &paths);
    ~Engine();

    void initialize();
    void run(const std::string &language, const std::string &file, std::string &args);
};

#endif