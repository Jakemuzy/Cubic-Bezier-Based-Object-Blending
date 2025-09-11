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
        auto input = std::make_unique<InputHandler>(renderer, camera);

        GLFWwindow *window = static_cast<GLFWwindow*>(renderer->GetWindow()->GetNativeHandle());
        Camera* cam = static_cast<Camera*>(camera);

        input->AttachKeyEvent( [=](){ cam->ProcessKeyboard(FORWARD, *deltaTime); }, GLFW_KEY_W );
        input->AttachKeyEvent( [=](){ cam->ProcessKeyboard(LEFT, *deltaTime); }, GLFW_KEY_A );
        input->AttachKeyEvent( [=](){ cam->ProcessKeyboard(BACKWARD, *deltaTime); }, GLFW_KEY_S );
        input->AttachKeyEvent( [=](){ cam->ProcessKeyboard(RIGHT, *deltaTime); }, GLFW_KEY_D );
        input->AttachKeyEvent( [=](){ cam->ProcessKeyboard(UP, *deltaTime); }, GLFW_KEY_SPACE );
        input->AttachKeyEvent( [=](){ cam->ProcessKeyboard(DOWN, *deltaTime); }, GLFW_KEY_LEFT_SHIFT );
        
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