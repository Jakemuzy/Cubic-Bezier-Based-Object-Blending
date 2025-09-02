#ifndef _IWINDOW_H__
#define _IWINDOW_H__

class IWindow
{
public:
    // Opengl, vulkan, sdl, sfml, etc window getter
    virtual void *GetNativeHandle() = 0; 
    virtual ~IWindow() = default;
};

#endif