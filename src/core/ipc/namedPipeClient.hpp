#pragma once

#include "namedPipe.hpp"

#include <string>

class NamedPipeClient : public NamedPipe
{
public:
    NamedPipeClient(std::string pipeName);
};
