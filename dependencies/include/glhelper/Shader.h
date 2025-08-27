#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
    Essentially a wrapper class for opengl Shaders, 
    allows for automatic assigning and handling of files
    based on input path alone
*/

class Shader
{
private:
    //  Converts file to const char for shader info
    std::string ReadFileData(std::string path);
public:
    unsigned int ID;

    Shader();
    Shader(std::string vPath, std::string fPath);

    void SetVertPath(std::string path);
    void SetFragPath(std::string path);

    void Use();

    // Utility uniform functions
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;

    void SetVec3(const std::string &name, std::vector<float> values) const;
    void SetVec3(const std::string &name, glm::vec3 values) const;
    void SetVec3(const std::string &name, float values[3]) const;
    void SetMat4(const std::string &name, glm::mat4 transformMatrix);
};

//  All Shaders
class ShaderGroup
{
private:
    std::vector<Shader> shaders;
public:
    ShaderGroup();
};

#endif