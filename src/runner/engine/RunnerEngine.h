#pragma once

#include "../runtime/RuntimeManager.h"
#include "../../core/Paths.h"
#include "../../core/eventBus/EventBus.h"

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
};
