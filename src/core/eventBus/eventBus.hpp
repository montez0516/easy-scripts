#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <spdlog/spdlog.h>

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <typeindex>

struct Event
{
    std::string to;
    std::string from;
    std::string type;
    virtual ~Event() = default;
};

class EventBus
{
private:
    std::map<std::type_index, std::vector<std::function<void(Event &)>>> subscribers;

public:
    template <typename T>
    void subscribe(std::function<void(T &)> callback)
    {
        auto typeIndx = std::type_index(typeid(T));
        subscribers[typeIndx].push_back([callback](Event &e)
                                        { callback(static_cast<T &>(e)); });
    }

    template <typename T>
    void publish(T &event)
    {
        std::string typeName = typeid(T).name();
        spdlog::debug("EventBus: FINDING CALLBACK FOR TYPE {}", typeName);
        auto typeIndx = std::type_index(typeid(T));
        if (subscribers.find(typeIndx) != subscribers.end())
        {
            spdlog::debug("EventBus: CALLING CALLBACKS FOR TYPE {}", typeName);
            for (auto &callback : subscribers[typeIndx])
            {
                callback(event);
            }
            spdlog::debug("EventBus: CALLED CALLBACKS FOR TYPE {}", typeName);
        }
        else
            spdlog::debug("EventBus: no callbacks found for type {}", typeName);
    }
};

#endif