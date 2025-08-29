#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <iostream>
#include <functional>

//  Shader includes all opengl headers
#include "Service.h"
#include "Shader.h"

/*
    This class is designed to be a sort of OpenGL "wrapper", that 
    encapsulates the functionality of rendering, but handles all of
    the regarding logic, and automaticlaly updates  when presented 
    certain data from the other classes.
*/

class GraphicsRenderer : public Service
{
private:
    GLFWwindow *window;
    int windowWidth = 1200, windowHeight = 800;

    //  Store shaders 
    std::unordered_map<std::string, Shader> shaderMap;
public:

    //  Setup OpenGL context, settings, etc
    GraphicsRenderer();
    ~GraphicsRenderer();

    //  Getters setters
    bool GetWindowCloseStatus(); 
    Shader GetShader(std::string shaderName);
    GLFWwindow *GetWindow();

    void UseShader(std::string shaderName);

    //  Attaching Callbacks, function pointer
    void AttachCallBack(std::function<void()> _callback);
    void AttachShader(std::string vPath, std::string fPath, std::string shaderName);

    //  Enable OpenGL processes ie.) glEnable(GL_MULTISAMPLE);
    void EnableSettings(std::initializer_list<GLenum> settings);
    void DisableSettings(std::initializer_list<GLenum> settings);

    //  ie.) GL_COLOR_BUFFER_BIT...
    void ClearBuffer(std::initializer_list <GLbitfield> buffers);

    

    //  Send events that'll trigger draw
    void Draw();
    //  Send event that will generate VAO, VBO, etc
    void GenerateVAO();
    void GenerateVAOEBO();

    //  Per frame code 
    void Update();

};

#endif