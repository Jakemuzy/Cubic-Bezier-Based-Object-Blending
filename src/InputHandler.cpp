#include "InputHandler.h"

InputHandler::InputHandler(IRenderer* gr) : window(static_cast<GLFWwindow*>(gr->GetWindow()->GetNativeHandle())) 
{
    glfwSetWindowUserPointer(window, this);

    OpenGLRenderer *glRender = static_cast<OpenGLRenderer*>(gr);
    glRender->AttachMouseCallback(MouseCallbackTrampoline);
}

void InputHandler::AttachKeyboardProcess(std::function<void()> _function)
{
    KeyboardFunctions.Subscribe(_function);
}

MousePos InputHandler::GetMousePos()
{
    MousePos pos(xPos, yPos);
    return pos;
}

void InputHandler::ProcessInput()
{
    // Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    KeyboardFunctions.RaiseEvent();
}

