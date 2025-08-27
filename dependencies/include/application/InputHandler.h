#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__

//  Not sure if I should define them here or include GraphicsRenderer.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EventHandler.h"

/*
    Singleton to handle all the user input 
*/

class InputHandler
{
private:
    GLFWwindow *window;
    std::vector<Event> KeyboardFunctions;
    std::vector<Event> MouseFunctions;
public:
    InputHandler(GLFWwindow *_window) : window(_window) { }

    void AttachKeyboardProcess(Event _function);
    void AttachMouseProcess(Event _function);

    void ProcessInput();    
};

#endif

/*
void processInput(GLFWwindow* window)
{
    //Escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
        std::cout << "Closed Window\n";
    }

    // Move
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && timeSinceLastCursorFocus >= 0.4f)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            disableMouseMovement = false;
            firstMouse = true;
        }
        else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            disableMouseMovement = true;
        }

        timeSinceLastCursorFocus = 0;
    }
    timeSinceLastCursorFocus += deltaTime;
}
*/