#ifndef _ICAMERA_H__
#define _ICAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement
{
    RIGHT,
    LEFT,
    BACKWARD,
    FORWARD,
    UP,
    DOWN
};

class ICamera
{
private:
public:
    ~ICamera() = default;
    
    virtual glm::mat4 GetViewMatrix() = 0;
    virtual glm::mat4 GetProjMatrix() = 0;
    virtual glm::mat4* GetViewMatrixPtr() = 0;
    virtual glm::mat4* GetProjMatrixPtr() = 0;

    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;
    virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) = 0;

    virtual void Update() = 0;
};

#endif