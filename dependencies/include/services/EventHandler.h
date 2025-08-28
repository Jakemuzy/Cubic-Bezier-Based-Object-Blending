#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#include <functional>

//  My stuff
#include "Service.h"

class Event : public Service
{
private:
    std::vector<std::function<void()>> events;

public:
    Event();
    Event(std::function<void()> &&_event);

    void Subscribe(std::function<void()> &&_event);
    void Unsubscribe();     //  Idk yet, maybe a map
    void RaiseEvent();
};  

#endif