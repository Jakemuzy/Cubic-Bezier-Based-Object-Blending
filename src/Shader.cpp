#include "Shader.h"

Shader::Shader()
{

}

Shader::Shader(std::string vPath, std::string fPath)
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
void Shader::SetVec3(const std::string &name, std::vector<float> values) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), values[0], values[1], values[2]);
}
void Shader::SetVec3(const std::string &name, glm::vec3 values) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), values[0], values[1], values[2]);
}
void Shader::SetMat4(const std::string &name, glm::mat4 transformMatrix)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transformMatrix));
}

//  -- Helper --
std::string Shader::ReadFileData(std::string path)
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