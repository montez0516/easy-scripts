#include "EventBus.h"
#include "../communication/message.h"

#include <spdlog/spdlog.h>

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <mutex>
#include <utility>

std::uint64_t EventBus::subscribe(const std::string &name, Method method)
{
    std::lock_guard lock(subscriptionMutex_);
    std::uint64_t id = idCounter_++;
    Subscription s;
    s.id = id;
    s.method = std::move(method);
    subscriptions_[name].push_back(std::move(s));
    return id;
}

void EventBus::publish(
    const std::string &name,
    const Message &message)
{
    std::vector<Subscription> subscriptions;

    {
        std::lock_guard lock(subscriptionMutex_);
        if (!subscriptions_.contains(name))
            return;

        subscriptions = subscriptions_.at(name);
    }

    for (Subscription &subscription : subscriptions)
    {
        subscription.method(message);
    }
}

void EventBus::unsubscribe(const std::string &name, std::uint64_t id)
{
    std::lock_guard lock(subscriptionMutex_);
    if (!subscriptions_.contains(name))
        return;

    std::vector<Subscription> &subscriptions = subscriptions_.at(name);

    auto it = std::find_if(subscriptions.begin(), subscriptions.end(), [id](const Subscription &s)
                           { return s.id == id; });

    if (it == subscriptions.end())
    {
        spdlog::debug("Method with id {} not found", id);
        return;
    }

    subscriptions.erase(it);

    if (subscriptions.empty())
    {
        subscriptions_.erase(name);
    }
}