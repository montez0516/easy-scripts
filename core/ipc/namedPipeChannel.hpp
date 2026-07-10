#ifndef NAMED_CHANNEL_H
#define NAMED_CHANNEL_H

#include <windows.h>
#include <string>

class NamedPipeChannel
{
public:
    NamedPipeChannel(const std::string &);
};

#endif