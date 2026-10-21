#pragma once

#include "../communication/Message.h"

#include <spdlog/spdlog.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cstdint>
#include <atomic>
#include <mutex>

using Method = std::function<void(const Message &)>;
struct Subscription
{
    std::uint64_t id;
    Method method;
};

class EventBus
{
public:
    std::uint64_t subscribe(const std::string &name, Method method);
    void publish(const std::string &name, const Message &message);
    void unsubscribe(const std::string &name, std::uint64_t id);

private:
    std::uint64_t idCounter_;
    std::mutex subscriptionMutex_;
    std::unordered_map<std::string, std::vector<Subscription>> subscriptions_;
};
