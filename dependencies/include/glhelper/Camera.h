#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "ICamera.h"


/*
    This class is meant to act as a camera using matrix
    transformations, emulates the functionality of a 
    camera
*/


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera : public ICamera
{
private:
    void UpdateCameraVectors();

    glm::mat4 proj = glm::perspective((float)glm::radians(60.0f), (float)1920.0f / (float)1080, 0.1f, 100.0f);
    glm::mat4 view;
public:
    glm::vec3 Position, Front, Up, Right, WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera() = default;   //  Throw error if unitalized
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix() { return view; }
    glm::mat4 GetProjMatrix() { return proj; }
    glm::mat4* GetViewMatrixPtr() { return &view; }
    glm::mat4* GetProjMatrixPtr() { return &proj; }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) override;

    void Update() { view = glm::lookAt(Position, Position + Front, Up); }
};

#endif