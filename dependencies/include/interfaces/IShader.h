#ifndef _ISHADER_H__
#define _ISHADER_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class IShader
{
private:
public:
    virtual void Use() = 0;
    virtual ~IShader() = default;

    virtual unsigned int GetID() = 0;

    // Utility uniform functions
    virtual void SetBool(const std::string &name, bool value) const = 0;
    virtual void SetInt(const std::string &name, int value) const = 0;
    virtual void SetFloat(const std::string &name, float value) const = 0;

    virtual void SetVec3(const std::string &name, const glm::vec3& values) const = 0;
    //virtual void SetVec3(const std::string &name, const float values[3]) = 0;
    virtual void SetMat4(const std::string &name, const glm::mat4& transformMatrix) = 0;
};


#endif 