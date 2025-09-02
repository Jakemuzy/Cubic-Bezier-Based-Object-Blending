#ifndef _OPENGL_WINDOW_H__
#define _OPENGL_WINODW_H__

#include "IWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLWindow : public IWindow
{
private:
    GLFWwindow *window;

public:
    OpenGLWindow(GLFWwindow *w) : window(w) {}
    void *GetNativeHandle() override { return window; }
};

#endif