#include "MessageCodec.h"
#include "Message.h"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <string>

std::string MessageCodec::encode(const Message &message)
{
    nlohmann::json json = nlohmann::json({{"type", message.type},
                                          {"scriptName", message.scriptName},
                                          {"promiseID", message.promiseID},
                                          {"data", message.data}});

    return json.dump();
}

Message MessageCodec::decode(std::string_view data)
{
    try
    {

        nlohmann::json json = nlohmann::json::parse(data);
        return {
            .type = json.at("type").get<std::string>(),
            .scriptName = json.at("scriptName").get<std::string>(),
            .promiseID = json.value("promiseID", std::string{}),
            .data = json.value("data", nlohmann::json::object())

        };
    }
    catch (nlohmann::json::exception &e)
    {
        spdlog::error("MessageCodec(decode): Failed to parse message into json {}\n{}", e.what(), data);
    }
    return {};
}