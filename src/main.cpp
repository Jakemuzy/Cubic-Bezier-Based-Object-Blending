#define STB_IMAGE_IMPLEMENTATION

// imgui
/*
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
*/

// std
#include <iostream>
#include <vector>
#include <filesystem>

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
#include "ServiceProvider.h"
#include "GraphicsRenderer.h"   // Must be declared first as its an opengl wrapper
#include "InputHandler.h"
#include "Camera.h"

// assimp
#include "Model.h"

int main(void)
{ 
    ServiceProvider serviceProvider;
    
    GraphicsRenderer renderer = serviceProvider.GetService<GraphicsRenderer>();
    InputHandler     input    = serviceProvider.GetService<InputHandler>();
    Camera           cam      = serviceProvider.GetService<Camera>();

    Model backpack("C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\backpack", "\\backpack.obj");
    //Model treestump("C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\chopping-log", "\\chopping-log.glb");

    glm::mat4 backpackModel = glm::mat4(1.0f);
    glm::mat4 treestumpModel = glm::mat4(1.0f);
    treestumpModel = glm::rotate(treestumpModel, glm::radians(90.0f), glm::vec3(-1, 0, 0));
    treestumpModel = glm::translate(treestumpModel, glm::vec3(-5, 0, 3.5));

    glm::mat4 projection = glm::perspective((float)glm::radians(60.0f), (float)1920.0f / (float)1080, 0.1f, 100.0f);

    // Main Loop
    while (!renderer.GetWindowCloseStatus())
    {
        serviceProvider.UpdateDeltaTime();


        input.ProcessInput();

        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
        renderer.ClearBuffer({GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT});

        renderer.UseShader("BlockShader");
        glm::mat4 view = cam.GetViewMatrix();

        renderer.GetShader("BlockShader").SetMat4("model", backpackModel);
        renderer.GetShader("BlockShader").SetMat4("view", view);
        renderer.GetShader("BlockShader").SetMat4("projection", projection);
        backpack.Draw(renderer.GetShader("BlockShader"));

        renderer.GetShader("BlockShader").SetMat4("model", treestumpModel);
        //treestump.Draw(renderer.GetShader("BlockShader"));

        /*
            DataContext.UpdateFrame();
        */

        renderer.Update();
    }

    return 0;
}  