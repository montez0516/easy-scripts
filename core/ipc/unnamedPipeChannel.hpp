#ifndef UNNAMED_CHANNEL_H
#define UNNAMED_CHANNEL_H

#include "unnamedPipe.hpp"

class UnnamedPipeChannel
{
private:
public:
    UnnamedPipe *stdinPipe;
    UnnamedPipe *stdoutPipe;
    UnnamedPipeChannel();
    ~UnnamedPipeChannel();
};

#endif