#ifndef _APPLICATION_FACTORY_H__
#define _APPLICATION_FACTORY_H__

#include <memory>

#include "OpenGLRenderer.h"
#include "InputHandler.h"
#include "Camera.h"

class RendererFactory
{
public:
    static std::unique_ptr<IRenderer> CreateOpenGLRenderer()
    {
        return std::make_unique<OpenGLRenderer>();
    }
};

class InputFactory
{
public:
    static std::unique_ptr<IInput> CreateInputHandler(IRenderer* renderer, ICamera* camera, const float* deltaTime)
    {
        auto input = std::make_unique<InputHandler>(renderer);

        GLFWwindow *window = static_cast<GLFWwindow*>(renderer->GetWindow()->GetNativeHandle());
        Camera* cam = static_cast<Camera*>(camera);

        std::function<void()> movement = [=]{
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cam->ProcessKeyboard(FORWARD, *deltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cam->ProcessKeyboard(BACKWARD, *deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cam->ProcessKeyboard(LEFT, *deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cam->ProcessKeyboard(RIGHT, *deltaTime);
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                cam->ProcessKeyboard(UP, *deltaTime);
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cam->ProcessKeyboard(DOWN, *deltaTime); 
        };
        

        input->AttachKeyboardProcess(movement);

        return input;
    }
};

class CameraFactory
{
public:
    static std::unique_ptr<ICamera> CreateCamera()
    {
        return std::make_unique<Camera>(glm::vec3(0, 0, 4), glm::vec3(0, 1, 0), -90.0f, 0.0f);
    }
};

#endif