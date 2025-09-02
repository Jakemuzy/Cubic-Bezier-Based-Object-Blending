#include "Shader.h"


Shader::Shader(const std::string& vPath, const std::string& fPath)
{
    std::string vertPath = ReadFileData(vPath);
    std::string fragPath = ReadFileData(fPath);

    const char * vertexShader = vertPath.c_str();
    const char * fragmentShader = fragPath.c_str();

    //  Vertex
    unsigned int vShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vShader, 1, &vertexShader, NULL);
    glCompileShader(vShader);

    //  Fragment
    unsigned int fShader;
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fShader, 1, &fragmentShader, NULL);
    glCompileShader(fShader);

    //  Shader Program
    ID = glCreateProgram();

    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glLinkProgram(ID);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}



void Shader::Use()
{
    glUseProgram(ID);
}



void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetVec3(const std::string &name, const glm::vec3& values) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), values[0], values[1], values[2]);
}
void Shader::SetMat4(const std::string &name, const glm::mat4& transformMatrix)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transformMatrix));
}
