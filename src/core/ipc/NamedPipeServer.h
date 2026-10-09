#pragma once

#include "NamedPipe.h"

#include <string>

class NamedPipeServer : public NamedPipe
{
public:
    NamedPipeServer(std::string pipeName);
    bool waitForConnection();
};
