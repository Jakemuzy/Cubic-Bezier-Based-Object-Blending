#include "EventHandler.h"

Event::Event()
{
    
}

Event::Event(std::function<void()> &&_event)
{
    Subscribe(std::move(_event));
}

void Event::Subscribe(std::function<void()> &&_event)
{
    events.emplace_back(std::move(_event));
}
void Event::RaiseEvent()
{
    for (const auto &event : events)
    {
        event();
    }
}