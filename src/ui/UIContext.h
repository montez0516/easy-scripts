#pragma once

#include "../core/Paths.h"
#include "../core/eventBus/EventBus.h"
#include "../api/Register.h"

struct UIContext
{
    Paths &paths;
    EventBus &eventBus;
    APIRegister &apiRegister;
};