#include "Register.h"

#include <string>
#include <string_view>
#include <functional>

bool APIRegister::registerMethod(std::string name, APIMethod method)
{
    if (!method)
        return false;

    return methods_.emplace(std::move(name), std::move(method)).second;
}

bool APIRegister::unregisterMethod(std::string_view name)
{
    return methods_.erase(std::string(name)) != 0;
}

bool APIRegister::hasMethod(std::string_view name) const
{
    return methods_.contains(std::string(name));
}

APIResponse APIRegister::call(std::string_view name, const APIRequest &request) const
{
    auto it = methods_.find(std::string(name));

    if (it == methods_.end())
    {
        return APIResponse::failure("Unknown API method: " + std::string(name));
    }

    try
    {
        return it->second(request);
    }
    catch (const std::exception &e)
    {
        return APIResponse::failure(e.what());
    }
    catch (...)
    {
        return APIResponse::failure("Unknown API error");
    }
}