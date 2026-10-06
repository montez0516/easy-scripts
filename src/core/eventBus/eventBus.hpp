#pragma once

#include "../communication/message.hpp"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <string>
#include <map>
#include <vector>
#include <functional>

class EventBus
{
public:
    using Method = std::function<void(const Message &)>;

    void subscribe(const std::string &name, Method method);
    void publish(const std::string &name, const Message &message);

private:
    std::map<std::string, std::vector<Method>> methods_;
};
