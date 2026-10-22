#ifndef MESSAGE_H
#define MESSAGE_H
#include <nlohmann/json.hpp>

#include <string>
#include <cstdint>
#include <map>

struct Message
{
    std::string type;
    std::string scriptName;
    std::string promiseID;
    nlohmann::json data;
};

#endif