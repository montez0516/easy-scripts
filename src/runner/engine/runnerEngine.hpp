#ifndef RUNNER_ENGINE_H
#define RUNNER_ENGINE_H

#include "../runtime/runtimeManager.hpp"
#include "../../core/paths.hpp"
#include "../../core/eventBus/eventBus.hpp"

#include <vector>
#include <string>

class Engine
{

private:
    RuntimeManager runtimeManager_;
    Paths &paths_;
    EventBus &bus_;
public:
    Engine(Paths &paths, EventBus &bus);

    void initialize();
    void run(const std::string &language, const std::string &file, std::string &args);
};

#endif