#pragma once

#include "NamedPipe.h"

#include <string>

class NamedPipeClient : public NamedPipe
{
public:
    NamedPipeClient(std::string pipeName);
};
