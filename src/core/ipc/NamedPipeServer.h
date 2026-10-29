#pragma once

#include "NamedPipe.h"

#include <string>

class NamedPipeServer : public NamedPipe
{
public:
    NamedPipeServer(const std::string &pipeName);
    bool waitForConnection();
};
