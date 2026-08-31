#ifndef EVENTS_H
#define EVENTS_H

#include <nlohmann/json.hpp>

#include <string>
#include <vector>


namespace Events{

    std::string response(const std::string &to, const nlohmann::json &payload)
    {
        return nlohmann::json({
            {"to",to},
            {"type","response"},
            {"payload", payload}
        }).dump();
    }

    std::string run(const std::string &file, const std::string &language, const std::vector<std::string> &args)
    {
        return nlohmann::json({
            {"type","run"},
            {"payload", {
                {"file",file},
                {"language",language},
                {"args",args}
            }}
        }).dump();
    }

}

#endif