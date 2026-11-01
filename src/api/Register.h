#pragma once

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <string>
#include <string_view>
#include <functional>
#include <unordered_map>
#include <utility>

struct APIRequest
{
    nlohmann::json params = nlohmann::json::object();
};
struct APIResponse
{
    bool status;
    nlohmann::json result;
    std::string error;
    static APIResponse success(nlohmann::json result = nullptr)
    {
        return {
            .status = true,
            .result = std::move(result),
            .error = ""};
    }

    static APIResponse failure(std::string error)
    {
        return {
            .status = false,
            .result = nullptr,
            .error = std::move(error)};
    }
};

class APIRegister
{

public:
    using APIMethod = std::function<APIResponse(const APIRequest &)>;
    bool registerMethod(std::string name, APIMethod method);
    bool unregisterMethod(std::string_view name);
    bool hasMethod(std::string_view name) const;

    APIResponse call(std::string_view name, const APIRequest &request) const;

private:
    std::unordered_map<std::string, APIMethod> methods_;
};
