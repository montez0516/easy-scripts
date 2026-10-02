#include "pipe.hpp"

#include <spdlog/spdlog.h>

void Pipe::closeRead()
{
    spdlog::critical("Pipe(closeRead): Function not implemented");
}

void Pipe::closeWrite()
{
    spdlog::critical("Pipe(closeWrite): Function not implemented");
}

bool Pipe::waitForConnection()
{
    spdlog::critical("Pipe(waitForConnection): Function not implemented");
    return false;
}