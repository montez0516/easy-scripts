#ifndef RUNNER_ENGINE_H
#define RUNNER_ENGINE_H

#include "../runtime/runtimeManager.hpp"
#include <vector>
#include <string>

class Engine
{

private:
    RuntimeManager *runtimeManager = new RuntimeManager();

public:
    ~Engine();

    void initialize();
    void run(const std::string &language, const std::string &file, std::string &args);
};

#endif