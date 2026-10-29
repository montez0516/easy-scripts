#pragma once

#include "NamedPipe.h"

#include <string>

class NamedPipeClient : public NamedPipe
{
public:
    NamedPipeClient(const std::string &pipeName);
};
