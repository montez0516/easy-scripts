#ifndef MESSAGE_H
#define MESSAGE_H
#include <nlohmann/json.hpp>

#include <string>
#include <cstdint>
#include <map>

enum class MessageType
{
    Request = 1,
    Response,
    Event,
    Unknown
};

struct Message
{
    MessageType type;
    std::string name;
    std::string id;
    nlohmann::json data;
};

constexpr MessageType messageTypeFromString(int type)
{
    if (type == 1)
        return MessageType::Request;
    else if (type == 2)
        return MessageType::Response;
    else if (type == 3)
        return MessageType::Event;
    return MessageType::Unknown;
}

#endif