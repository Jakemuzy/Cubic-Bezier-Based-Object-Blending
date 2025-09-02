#ifndef _IINPUT_H__
#define _IINPUT_H__

#include "EventHandler.h"

class IInput
{
private:
public:
    virtual void AttachKeyboardProcess(std::function<void()> _function) = 0;
    virtual void ProcessInput() = 0;
};

#endif