#pragma once

#include "Message.h"
#include <string>

class MessageCodec
{
public:
    static std::string encode(const Message &message);
    static Message decode(std::string_view data);
};