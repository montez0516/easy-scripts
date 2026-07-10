#include <windows.h>
#include <iostream>
#include <string>

#include "unnamedPipeChannel.hpp"

UnnamedPipeChannel::UnnamedPipeChannel() : stdinPipe(new UnnamedPipe()), stdoutPipe(new UnnamedPipe())
{
}

UnnamedPipeChannel::~UnnamedPipeChannel()
{
    delete stdinPipe;
    delete stdoutPipe;
}