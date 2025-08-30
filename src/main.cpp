#define STB_IMAGE_IMPLEMENTATION

// my stuff
#include "ServiceProvider.h"

// assimp
#include "Model.h"
#include "Octree.h"

int main(void)
{ 
    ServiceProvider serviceProvider;
    
    GraphicsRenderer& renderer = serviceProvider.GetService<GraphicsRenderer>();
    InputHandler&     input    = serviceProvider.GetService<InputHandler>();
    Camera&           cam      = serviceProvider.GetService<Camera>();

    Model backpack("C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\backpack", "\\backpack.obj");
    //Model treestump("C:\\Users\\jmuzy\\OneDrive\\Desktop\\Projects\\Object Blending\\chopping-log", "\\chopping-log.glb");

    renderer.AttachShader("dependencies\\shaders\\Block.vert", "dependencies\\shaders\\Block.frag", "BlockShader");
    renderer.AttachShader("dependencies\\shaders\\Octree.vert", "dependencies\\shaders\\Octree.frag", "OctreeShader");

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

        //  Change this later
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        renderer.UseShader("OctreeShader");
        renderer.GetShader("OctreeShader").SetMat4("model", backpackModel);
        renderer.GetShader("OctreeShader").SetMat4("view", view);
        renderer.GetShader("OctreeShader").SetMat4("projection", projection);
        backpack.DrawOctree(renderer.GetShader("OctreeShader"));
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        
        //  Backpack.octree.draw(;)

        /*
            DataContext.UpdateFrame();
        */

        renderer.Update();
        
    }

    return 0;
}  