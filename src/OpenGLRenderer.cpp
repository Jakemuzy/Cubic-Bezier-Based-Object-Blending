#include "OpenGLRenderer.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
}
void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{

}




//  Constructor / Destructors
OpenGLRenderer::OpenGLRenderer()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *nativeWindow = glfwCreateWindow(windowWidth, windowHeight, "Minecraft", NULL, NULL);
    window = new OpenGLWindow(nativeWindow);

    // Callback functions
    glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(window->GetNativeHandle()), framebuffer_size_callback); // Call the function, every window resize
    glfwSetCursorPosCallback(static_cast<GLFWwindow *>(window->GetNativeHandle()), mouseCallback);

    //  Window
    if (nativeWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        abort();
    }
    glfwMakeContextCurrent(static_cast<GLFWwindow *>(window->GetNativeHandle()));

    //  Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        abort();
    }

    //  Viewport
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetInputMode(static_cast<GLFWwindow *>(window->GetNativeHandle()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //  Settings
    EnableSettings({GL_MULTISAMPLE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_3D});
    // glEnable(GL_POLYGON_MODE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
}

OpenGLRenderer::~OpenGLRenderer()
{
    delete(window);
    glfwTerminate();
}

//  Getters / Setters
bool OpenGLRenderer::GetWindowCloseStatus()
{
    return glfwWindowShouldClose(static_cast<GLFWwindow *>(window->GetNativeHandle()));
}
OpenGLWindow* OpenGLRenderer::GetWindow()
{
    return window;
}
Shader* OpenGLRenderer::GetShader(const std::string& shaderName)
{
    return &shaderMap[shaderName];
}
float* OpenGLRenderer::GetDeltaTime()
{
    return &deltaTime;
}

void OpenGLRenderer::UseShader(const std::string& shaderName)
{
    auto shader = shaderMap.find(shaderName);
    if(shader == shaderMap.end())
    {
        std::cout << "ERROR: attempted to use an undefined shader\n";
    }
    
    shaderMap[shaderName].Use();
}

void OpenGLRenderer::AttachShader(std::string vPath, std::string fPath, std::string shaderName)
{
    //  Use emplace 
    shaderMap[shaderName] = Shader(vPath, fPath);
}
void OpenGLRenderer::AttachMouseCallback(void (*mouseCallback)(GLFWwindow *, double, double))
{
    glfwSetCursorPosCallback(static_cast<GLFWwindow *>(window->GetNativeHandle()), mouseCallback);
}

//  Settings
void OpenGLRenderer::EnableSettings(std::initializer_list<GLenum> settings)
{
    for(auto& setting : settings)
        glEnable(setting);
}
void OpenGLRenderer::DisableSettings(std::initializer_list<GLenum> settings)
{
    for(auto& setting : settings)
        glDisable(setting);
}

//  Buffers
void OpenGLRenderer::ClearBuffer(uint32_t buffers)
{
    glClear(buffers);
}

//  Updates
void OpenGLRenderer::Update()
{
    currentFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    glfwSwapBuffers(static_cast<GLFWwindow *>(window->GetNativeHandle()));
    glfwPollEvents();
}
