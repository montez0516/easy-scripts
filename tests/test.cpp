#include "../src/core/eventBus/eventBus.hpp"
#include <iostream>

void TestEventBus()
{
    EventBus bus;

    bus.subscribe<Event>([](const Event& e){
        if(e.id != "main")
        {
           std::cout << "Event id is incorrect " <<e.id << "expected \"main\"" << std::endl; 
        }
        else
            std::cout << "Event id is correct " << e.id << std::endl;
        if(e.type != "message")
        {
            std::cout << "Event type is incorrect " << e.type << "expected \"message\"" << std::endl;
        }
        else
            std::cout << "Event type is correct " << e.type << std::endl;
    });

    Event e;
    e.id = "main";
    e.type = "message";
    bus.publish<Event>(e);
}

int main()
{
    TestEventBus();

    return 0;
}