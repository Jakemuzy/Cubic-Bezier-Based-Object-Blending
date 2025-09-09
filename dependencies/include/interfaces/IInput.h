#ifndef _IINPUT_H__
#define _IINPUT_H__

#include "EventHandler.h"

struct MousePos
{
    float x, y;
    MousePos(float _x, float _y) : x(_x), y(_y)  { }
};

class IInput
{
private:
public:
    virtual void AttachKeyEvent(std::function<void()> _function, unsigned int GLKey) = 0;

    virtual void ProcessInput() = 0;
    virtual MousePos GetMousePos() = 0;
};

#endif