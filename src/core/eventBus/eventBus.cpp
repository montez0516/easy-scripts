#include "eventBus.hpp"
#include "../communication/message.hpp"

void EventBus::subscribe(const std::string &name, Method method)
{
    methods_[name].push_back(method);
}

void EventBus::publish(const std::string &name, const Message &message)
{
    if (!methods_.contains(name))
    {
        return;
    }

    std::vector<Method> methods = methods_.at(name);

    for (Method &method : methods)
    {
        method(message);
    }
}
