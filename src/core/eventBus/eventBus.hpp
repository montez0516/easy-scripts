#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <typeindex>

struct Event{
    std::string to;
    std::string from;
    std::string type;
    virtual ~Event() = default;
};

class EventBus{
    private:
        std::map<std::type_index, std::vector<std::function<void(const Event&)>>> subscribers;

    public:

    template<typename T>
    void subscribe(std::function<void(T&)> callback)
    {
        auto typeIndx = std::type_index(typeid(T));
        subscribers[typeIndx].push_back([callback](const Event& e)
        {
            callback(static_cast<const T&>(e));
                }
            );
        }

    template<typename T>
    void publish(const T& event)
    {
        auto typeIndx = std::type_index(typeid(T));
        if(subscribers.find(typeIndx) != subscribers.end())
        {
            for(auto &callback : subscribers[typeIndx])
            {
                callback(event);
            }
        }
    }
};

#endif