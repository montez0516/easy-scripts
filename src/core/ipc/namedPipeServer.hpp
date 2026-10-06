#pragma once

#include "namedPipe.hpp"

#include <string>

class NamedPipeServer : public NamedPipe
{
public:
    NamedPipeServer(std::string pipeName);
    bool waitForConnection();
};
