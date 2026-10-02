#pragma once

#include "message.hpp"
#include <string>

class MessageCodec
{
public:
    static std::string encode(const Message &message);
    static Message decode(std::string_view data);
};