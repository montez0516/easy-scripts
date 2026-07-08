#ifndef IPC_CHANNEL_H
#define IPC_CHANNEL_H

#include <cstdint>
#include <string>
#include <vector>

struct Message
{
    uint32_t id;
    std::string type;
    std::vector<uint8_t> data;
};

class IPCChannel
{
public:
    virtual Message recieve() = 0;
    virtual void send(const Message &) = 0;
};

#endif