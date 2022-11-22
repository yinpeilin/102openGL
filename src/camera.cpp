#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
{
    Position = position;
    WorldUp = up;
    Up = glm::normalize(up);
    Right = glm::normalize(glm::cross(Front, Up));
    // updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
// updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime*100;
    // std::cout<<glm::to_string(Position)<<std::endl;
    switch (direction)
    {
    case Camera_Movement::FORWARD:
        Position += Front * velocity;
        break;
    case Camera_Movement::BACKWARD:
        Position -= Front * velocity;
        break;
    case Camera_Movement::LEFT:
        Position -= Right * velocity;
        break;
    case Camera_Movement::RIGHT:
        Position += Right * velocity;
        break;
    default:
        break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    updateCameraVectorsByQuat(xoffset, yoffset);
}
void Camera::ProcessMouseScroll(float yoffset)
{
    if (Fov >= 1.0f && Fov <= 85.0f)
        Fov -= yoffset*10;
    if (Fov <= 1.0f)
        Fov = 1.0f;
    if (Fov >= 85.0f)
        Fov = 85.0f;

}

void Camera::updateCameraVectorsByQuat(float xoffset, float yoffset)
{

    glm::vec3  ViewDest = Front + Right * xoffset * 0.008f * MouseSensitivity + Up * yoffset * 0.008f * MouseSensitivity;
    glm::quat rot1 = RotationBetweenVectors(Front, ViewDest);
    Front = glm::normalize(glm::rotate(rot1, Front));
    Right = glm::normalize(glm::cross(Front, Up));
    Up = glm::normalize(glm::cross(Right, Front));
}

glm::quat Camera::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
{
    start = normalize(start);
    dest = normalize(dest);
    float cosTheta = dot(start, dest);
    glm::vec3 rotationAxis;
    if (cosTheta < -1 + 0.001f) 
    {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start

        rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
        if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
            rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
        rotationAxis = normalize(rotationAxis);
        return glm::angleAxis(180.0f, rotationAxis);

    }
    rotationAxis = cross(start, dest);
    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;
    return glm::quat(s * 0.5f,rotationAxis.x * invs,rotationAxis.y * invs,rotationAxis.z * invs);
}