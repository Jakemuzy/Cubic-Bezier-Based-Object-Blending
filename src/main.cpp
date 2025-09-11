#define STB_IMAGE_IMPLEMENTATION

// my stuff
#include "ApplicationFactory.h"
#include "Scene.h"

int main(void)
{ 
    auto renderer = RendererFactory::CreateOpenGLRenderer();
    auto cam = CameraFactory::CreateCamera();   //  Gotta transfer event from input factory to camera for mouse movement
    auto input = InputFactory::CreateInputHandler(renderer.get(), cam.get(), renderer->GetDeltaTime());


    Scene scene(renderer.get());

    renderer->AttachShader("dependencies\\shaders\\Model.vert", "dependencies\\shaders\\Model.frag", "ModelShader");
    renderer->AttachShader("dependencies\\shaders\\Octree.vert", "dependencies\\shaders\\Octree.frag", "OctreeShader");

    glm::mat4 backpackModel = glm::mat4(1.0f);
    glm::mat4 girlModel = glm::translate(backpackModel, glm::vec3(3.0f, -1.0f, -0.5f));
    glm::mat4 projection = glm::perspective((float)glm::radians(60.0f), (float)1920.0f / (float)1080, 0.1f, 100.0f);

    glm::vec3 backpackPrevPos = glm::vec3(0.0f);

    scene.UpdateOctree("girl", girlModel[3]);
    scene.UpdateOctree("backpack", backpackModel[3]);

    std::function<void()> updateOctree = [&]() {
        glm::vec3 backpackCurrentPos = glm::vec3(backpackModel[3]); // extract translation
        glm::vec3 deltaMovement = backpackCurrentPos - backpackPrevPos;
        scene.UpdateOctree("backpack", deltaMovement);
        backpackPrevPos = backpackCurrentPos;
    };

    //  Update octree if it moves, same with model pos 
    std::function<void()> leftMove  = [&]{ 
        backpackModel = glm::translate(backpackModel, glm::vec3(-2.0 * (*renderer->GetDeltaTime()), 0, 0));
        updateOctree();
     };
    std::function<void()> rightMove = [&]{ 
        backpackModel = glm::translate(backpackModel, glm::vec3(2.0 * (*renderer->GetDeltaTime()), 0, 0));
        updateOctree();
    };
    std::function<void()> forwardMove = [&]{ 
        backpackModel = glm::translate(backpackModel, glm::vec3(0, 0, -2.0 * (*renderer->GetDeltaTime())));
        updateOctree();
    };
    std::function<void()> backwardMove = [&]{ 
        backpackModel = glm::translate(backpackModel, glm::vec3(0, 0, 2.0 * (*renderer->GetDeltaTime())));
        updateOctree();
    };
    
    input->AttachKeyEvent(leftMove, GLFW_KEY_LEFT);
    input->AttachKeyEvent(rightMove, GLFW_KEY_RIGHT);
    input->AttachKeyEvent(forwardMove, GLFW_KEY_UP);
    input->AttachKeyEvent(backwardMove, GLFW_KEY_DOWN);

    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main Loop
    while (!static_cast<OpenGLRenderer *>(renderer.get())->GetWindowCloseStatus())
    {
        static_cast<InputHandler*>(input.get())->ProcessInput();

        glClearColor(0.18f, 0.18f, 0.2f, 1.0f); 
        renderer->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = cam->GetViewMatrix();

        scene.DrawModel(backpackModel, view, projection, "backpack");
        scene.DrawModel(girlModel, view, projection, "girl");

        scene.CheckIntersections();
        renderer->Update();
    }

    return 0;
}  