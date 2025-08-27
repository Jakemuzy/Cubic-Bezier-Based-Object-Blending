#include "GraphicsRenderer.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
}
void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{

}

//  Constructor / Destructors
GraphicsRenderer::GraphicsRenderer()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Minecraft", NULL, NULL);

    // Callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Call the function, every window resize
    glfwSetCursorPosCallback(window, mouseCallback);

    //  Window
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        abort();
    }
    glfwMakeContextCurrent(window);

    //  Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        abort();
    }

    //  Viewport
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //  Settings
    EnableSettings({GL_MULTISAMPLE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_3D});
    // glEnable(GL_POLYGON_MODE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

GraphicsRenderer::~GraphicsRenderer()
{
    glfwTerminate();
}

//  Getters / Setters
bool GraphicsRenderer::GetWindowCloseStatus()
{
    return glfwWindowShouldClose(window);
}
GLFWwindow* GraphicsRenderer::GetWindow()
{
    return window;
}
Shader GraphicsRenderer::GetShader(std::string shaderName)
{
    return shaderMap[shaderName];
}
void GraphicsRenderer::UseShader(std::string shaderName)
{
    auto shader = shaderMap.find(shaderName);
    if(shader == shaderMap.end())
    {
        std::cout << "ERROR: attempted to use an undefined shader\n";
    }
    
    shaderMap[shaderName].Use();
}

//  Attaching Processes
void GraphicsRenderer::AttachCallBack(std::function<void()> _callback)
{
    
}
void GraphicsRenderer::AttachShader(std::string vPath, std::string fPath, std::string shaderName)
{
    //  Use emplace 
    shaderMap[shaderName] = Shader(vPath, fPath);
}

//  Settings
void GraphicsRenderer::EnableSettings(std::initializer_list<GLenum> settings)
{
    for(auto& setting : settings)
        glEnable(setting);
}
void GraphicsRenderer::DisableSettings(std::initializer_list<GLenum> settings)
{
    for(auto& setting : settings)
        glDisable(setting);
}

//  Buffers
void GraphicsRenderer::ClearBuffer(std::initializer_list<GLbitfield> buffers)
{
    for(auto& buffer : buffers)
        glClear(buffer);
}

//  Updates
void GraphicsRenderer::Update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

