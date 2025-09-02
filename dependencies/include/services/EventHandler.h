#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include <functional>
#include <vector>
#include <iostream>

template<typename... T>
class Event 
{
private:
    //  Make this a map
    std::vector<std::function<void(T...)>> events;

public:
    Event() = default;
    Event(std::function<void(T...)> &&_event)
    {
        Subscribe(std::move(_event));
    }
    Event(std::function<void(T...)> &_event)
    {
        Subscribe(std::move(_event));
    }

    ~Event() = default;
    Event(const Event &) = default;
    Event(Event &&) noexcept = default;
    Event &operator=(const Event &) = default;
    Event &operator=(Event &&) noexcept = default;

    void RaiseEvent(T... params)
    {
        if (events.empty())
        {
            std::cerr << "ERROR: Raise Event failed, no events attached\n";
            return;
        }

        for (const auto &event : events)
        {
            event(params...);
        }
    }
    void Subscribe(std::function<void(T...)> _event)
    {
        events.emplace_back(std::move(_event));
    }
    void Unsubscribe(); //  Implement later




    void operator +=(std::function<void(T...)> &&_event)
    {
        Subscribe(std::move(_event));
    }
    void operator +=(std::function<void(T...)> &_event)
    {
        Subscribe(_event);
    }

    void operator -=(std::function<void(T...)> &_event);        //  implement later
};  

#endif