#define STB_IMAGE_IMPLEMENTATION

// my stuff
#include "ApplicationFactory.h"
#include "Scene.h"

int main(void)
{ 
    auto renderer = RendererFactory::CreateOpenGLRenderer();
    auto cam = CameraFactory::CreateCamera();   //  Gotta transfer event from input factory to camera for mouse movement
    auto input = InputFactory::CreateInputHandler(renderer.get(), cam.get(), renderer->GetDeltaTime());

    Scene scene(renderer.get(), cam.get(), input.get());

    renderer->AttachShader("dependencies\\shaders\\Model.vert", "dependencies\\shaders\\Model.frag", "ModelShader");
    renderer->AttachShader("dependencies\\shaders\\Octree.vert", "dependencies\\shaders\\Octree.frag", "OctreeShader");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main Loop
    while (!static_cast<OpenGLRenderer *>(renderer.get())->GetWindowCloseStatus())
    {
        static_cast<InputHandler*>(input.get())->ProcessInput();

        glClearColor(0.18f, 0.18f, 0.2f, 1.0f); 
        renderer->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //  Update view matrix
        cam->Update();
        scene.Update();

        scene.CheckIntersections();
        renderer->Update();
    }

    return 0;
}  