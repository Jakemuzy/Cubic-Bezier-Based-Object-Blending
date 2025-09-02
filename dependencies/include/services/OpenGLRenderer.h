#ifndef __OPENGL_RENDERER_H__
#define __OPENGL_RENDERER_H__

#include <iostream>
#include <functional>

//  Shader includes all opengl headers
#include "IRenderer.h"

#include "EventHandler.h"
#include "OpenGLWindow.h"
#include "Shader.h"

/*
    This class is designed to be a sort of OpenGL "wrapper", that 
    encapsulates the functionality of rendering, but handles all of
    the regarding logic, and automaticlaly updates  when presented 
    certain data from the other classes.
*/

class OpenGLRenderer : public IRenderer
{
private:
    OpenGLWindow* window;
    int windowWidth = 1200, windowHeight = 800;
    float deltaTime = 0, currentFrameTime = 0, lastFrameTime = 0;   //  Make deltaTime unique_ptr

    //  Store shaders 
    std::unordered_map<std::string, Shader> shaderMap;

public:

    //  Setup OpenGL context, settings, etc
    OpenGLRenderer();
    ~OpenGLRenderer();

    //  Getters setters
    bool GetWindowCloseStatus() override; 
    Shader* GetShader(const std::string& shaderName) override;
    OpenGLWindow* GetWindow() override;
    float* GetDeltaTime() override;

    void UseShader(const std::string& shaderName) override;

    //  Attaching Callbacks, function pointer
    void AttachShader(std::string vPath, std::string fPath, std::string shaderName) override;
    void AttachMouseCallback(void (*mouseCallback)(GLFWwindow *, double, double));

    //  Enable OpenGL processes ie.) glEnable(GL_MULTISAMPLE);
    void EnableSettings(std::initializer_list<GLenum> settings);
    void DisableSettings(std::initializer_list<GLenum> settings);

    //  ie.) GL_COLOR_BUFFER_BIT...
    void ClearBuffer(uint32_t buffers) override;
    void Update();

};

#endif