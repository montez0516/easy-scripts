#ifndef PIPE_CHANNEL_H
#define PIPE_CHANNEL_H

#include <windows.h>

#include "IPCChannel.hpp"

class PipeChannel : public IPCChannel
{
private:
    HANDLE writeHandle;
    HANDLE readHandle;

public:
    PipeChannel();
    ~PipeChannel();
    Message recieve() override;
    void send(const Message &) override;
    HANDLE getWrite() const;
    HANDLE getRead() const;
};

#endif