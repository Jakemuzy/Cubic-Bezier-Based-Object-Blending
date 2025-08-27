#include "InputHandler.h"

void InputHandler::ProcessInput()
{
    // Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    for (auto &fucntion : KeyboardFunctions)
    {
        fucntion.RaiseEvent();
    }

    for (auto &function : MouseFunctions)
    {
        function.RaiseEvent();
    }

}

void InputHandler::AttachKeyboardProcess(Event _function)
{
    KeyboardFunctions.push_back(_function);
}

void InputHandler::AttachMouseProcess(Event _function)
{
    MouseFunctions.push_back(_function);
}