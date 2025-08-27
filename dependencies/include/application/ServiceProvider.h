#ifndef __SERVICE_PROVIDER_H__
#define __SERVICE_PROVIDER_H__

// imgui
/*
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
*/

// glad
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// etc help
#include <chrono>

// my stuff
#include "Service.h"    //  Must be declared first as all the services run on it

#include "GraphicsRenderer.h" // Must be declared as first service as its an opengl wrapper
#include "InputHandler.h"
#include "Camera.h"

// assimp
#include "Model.h"

/*
    This class's main purpose is to provide access to each of services,
    allowing for loose coupling of the data, and to query other classes
    data without knowledge of their inner workings. Additionally provides
    a centralized factory for generating the services.
*/


class ServiceProvider
{
private:
    std::tuple<
        std::unique_ptr<GraphicsRenderer>,
        std::unique_ptr<InputHandler>,
        std::unique_ptr<Camera>
    > services;

    float currentFrameTime = 0, lastFrameTime = 0, deltaTime = 0;
    bool firstMouse = true, disableMouseMovement = false;
    float lastX = 400, lastY = 300;

    void InitInputProcesses()
    {
        auto& renderer = GetService<GraphicsRenderer>();
        auto& input = GetService<InputHandler>();
        auto& cam = GetService<Camera>();

        input.AttachKeyboardProcess(Event([&]{
            if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
                cam.ProcessKeyboard(FORWARD, deltaTime);
            if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
                cam.ProcessKeyboard(BACKWARD, deltaTime);
            if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
                cam.ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
                cam.ProcessKeyboard(RIGHT, deltaTime); 
            })
        );

        /*
        input.AttachMouseProcess(
            -> get vec2 pos = renderer.MouseCallback
            -> based off of that pos, make event 
        );
        */
    }
public:
    ServiceProvider()
    {
        std::get<0>(services) = std::make_unique<GraphicsRenderer>();
        std::get<1>(services) = std::make_unique<InputHandler>(std::get<0>(services)->GetWindow());
        std::get<2>(services) = std::make_unique<Camera>(glm::vec3(0, 0, 4), glm::vec3(0, 1, 0), -90.0f, 0.0f);

        InitInputProcesses();
    }

    template<typename T>
    T& GetService()
    {
        return *std::get<std::unique_ptr<T>>(services);
    }

    void UpdateDeltaTime()
    {
        currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
    }


};


#endif