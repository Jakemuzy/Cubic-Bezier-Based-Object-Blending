#define STB_IMAGE_IMPLEMENTATION

// my stuff
#include "ApplicationFactory.h"
#include "Scene.h"

int main(void)
{ 
    auto renderer = RendererFactory::CreateOpenGLRenderer();
    auto cam = CameraFactory::CreateCamera();
    auto input = InputFactory::CreateInputHandler(renderer.get(), cam.get(), renderer->GetDeltaTime());

    Scene scene(renderer.get());

    renderer->AttachShader("dependencies\\shaders\\Block.vert", "dependencies\\shaders\\Block.frag", "BlockShader");
    renderer->AttachShader("dependencies\\shaders\\Octree.vert", "dependencies\\shaders\\Octree.frag", "OctreeShader");

    glm::mat4 backpackModel = glm::mat4(1.0f);
    glm::mat4 treestumpModel = glm::mat4(1.0f);
    treestumpModel = glm::rotate(treestumpModel, glm::radians(90.0f), glm::vec3(-1, 0, 0));
    treestumpModel = glm::translate(treestumpModel, glm::vec3(-5, 0, 3.5));

    glm::mat4 projection = glm::perspective((float)glm::radians(60.0f), (float)1920.0f / (float)1080, 0.1f, 100.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main Loop
    while (!static_cast<OpenGLRenderer *>(renderer.get())->GetWindowCloseStatus())
    {
        //  Update deltatime
        static_cast<InputHandler*>(input.get())->ProcessInput();
        //input->ProcessInput();

        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
        renderer->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = cam->GetViewMatrix();

        scene.Draw(backpackModel, view, projection);

        //  Change this later
        /*
        renderer.UseShader("OctreeShader");
        renderer.GetShader("OctreeShader").SetMat4("model", backpackModel);
        renderer.GetShader("OctreeShader").SetMat4("view", view);
        renderer.GetShader("OctreeShader").SetMat4("projection", projection);
        backpack.DrawOctree(renderer.GetShader("OctreeShader"));

        
        //  Backpack.octree.draw(;)
        */

        renderer->Update();
    }

    return 0;
}  