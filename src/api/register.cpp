#include "register.hpp"

bool APIRegister::registerMethod(std::string methodName, std::function<APIResponse(APIRequest)> method)
{
    if (hasMethod(methodName))
    {
        spdlog::error("APIRegister(registerMethod): method with name already exists {}", methodName);
        return false;
    }

    methods_[methodName] = std::move(method);
    return true;
}

void APIRegister::unregisterMethod(std::string methodName)
{
    methods_.erase(methodName);
}

std::function<APIResponse(APIRequest)> APIRegister::getMethod(std::string methodName)
{
    if (!hasMethod(methodName))
        return {};
    return methods_.at(methodName);
}

bool APIRegister::hasMethod(std::string methodName)
{
    return methods_.contains(methodName);
}