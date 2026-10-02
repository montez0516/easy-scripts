#include "messageCodec.hpp"
#include "message.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <string>

std::string MessageCodec::encode(const Message &message)
{
    nlohmann::json json = nlohmann::json({{"type", message.type},
                                          {"name", message.name},
                                          {"id", message.id},
                                          {"data", message.data}});

    return json.dump();
}

Message MessageCodec::decode(std::string_view data)
{
    try
    {

        nlohmann::json json = nlohmann::json::parse(data);
        return {
            .type = messageTypeFromString(json.value<int>("type", 0)),
            .name = json.at("name"),
            .id = json.value("id", ""),
            .data = json.value("data", nlohmann::json::object())};
    }
    catch (nlohmann::json::exception &e)
    {
        spdlog::error("MessageCodec(decode): Failed to parse message into json {}\n{}", e.what(), data);
    }
    return {};
}