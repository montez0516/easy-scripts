#ifndef REGISTER_H
#define REGISTER_H

#include <spdlog/spdlog.h>

#include <string>
#include <functional>
#include <unordered_map>
#include <utility>

struct APIResponse
{
    bool status;
    std::string result;
    std::string error;
};

struct APIRequest
{
    std::string params;
};

class APIRegister
{
private:
    std::unordered_map<std::string, std::function<APIResponse(APIRequest)>> methods_;

public:
    bool registerMethod(std::string methodName, std::function<APIResponse(APIRequest)> method);
    void unregisterMethod(std::string methodName);
    std::function<APIResponse(APIRequest)> getMethod(std::string methodName);
    bool hasMethod(std::string methodName);
};

#endif