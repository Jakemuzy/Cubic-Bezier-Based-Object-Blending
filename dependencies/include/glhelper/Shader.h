#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "IShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
    Essentially a wrapper class for opengl Shaders, 
    allows for automatic assigning and handling of files
    based on input path alone
*/

class Shader : public IShader
{
private:
    unsigned int ID;
public:
    

    Shader() = default;
    Shader(const std::string& vPath, const std::string& fPath);

    void Use() override;

    unsigned int GetID() override { return ID; }

    // Utility uniform functions
    void SetBool(const std::string &name, bool value) const override;
    void SetInt(const std::string &name, int value) const override;
    void SetFloat(const std::string &name, float value) const override;

    void SetVec3(const std::string &name, const glm::vec3& values) const override;
    //void SetVec3(const std::string &name, const float values[3]) override;
    void SetMat4(const std::string &name, const glm::mat4& transformMatrix) override;
};

//  Reads the file
static std::string ReadFileData(std::string path)
{
    std::ifstream shaderFile;
    std::string data;

    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path);

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        data = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << std::endl;
        std::cout << e.what() << "\n";
        std::cout << path << "\n";
    }

    return data;
}

#endif