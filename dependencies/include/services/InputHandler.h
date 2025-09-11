#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

//  Not sure if I should define them here or include GraphicsRenderer.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "IRenderer.h"
#include "IInput.h"
#include "ICamera.h"
#include "OpenGLRenderer.h"

/*
    Singleton to handle all the user input 
*/

class InputHandler : public IInput
{
private:
    GLFWwindow *window;
    Event<> KeyboardFunctions;
    Event<float, float> MouseFunctions;
    bool initalized = false;
    
    void HandleMouseMovement(double x, double y)
    {
        if (firstMouse)
        {
            lastX = x;
            lastY = y;
            firstMouse = false;
        }

        float xoffset = x - lastX;
        float yoffset = lastY - y;
        lastX = x;
        lastY = y;

        MouseFunctions.RaiseEvent(xoffset, yoffset);
    }

    //  Need this to bridge the gap
    static void MouseCallbackTrampoline(GLFWwindow *window, double xpos, double ypos)
    {
        InputHandler *handler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        if (handler)
            handler->HandleMouseMovement(xpos, ypos);
    }
    

public:
    //  Mouse
    //Event<double, double> mouseMoved;
    bool firstMouse = true, disableMouseMovement = false;
    float xPos, yPos;
    float lastX = 400, lastY = 300;

    InputHandler() { initalized = false; }   //  Make these processes error if trying to use while not inialized
    InputHandler(IRenderer* gr, ICamera* cam);

    void AttachKeyEvent(std::function<void()> _function, unsigned int GLKey) override;
    MousePos GetMousePos() override;

    void ProcessInput() override;    
};

#endif
