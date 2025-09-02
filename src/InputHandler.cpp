#include "InputHandler.h"

InputHandler::InputHandler(IRenderer* gr) : window(static_cast<GLFWwindow*>(gr->GetWindow()->GetNativeHandle())) 
{
    std::cout << "Input init\n";
    //glfwSetWindowUserPointer(window, this);

    //OpenGLRenderer *glRender = static_cast<OpenGLRenderer*>(gr);
    //glRender->AttachMouseCallback(MouseCallbackTrampoline);
}

void InputHandler::ProcessInput()
{
    // Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    std::cout << "pre raise\n";
    KeyboardFunctions.RaiseEvent();
    std::cout << "post raise\n";

}

void InputHandler::AttachKeyboardProcess(std::function<void()> _function)
{
    KeyboardFunctions.Subscribe(_function);
}
