#include "Camera.h"
#include <iostream>

Camera::Camera()
    :
    camera_position(glm::vec3(0.0, 0.0, 3.f)),
    camera_front(glm::vec3(0, 0, -1)),
    camera_up(glm::vec3(0, 1, 0)), camera_target(glm::vec3(0, 0, 0)),
    camera_direction(glm::normalize(camera_position - camera_target)),
    up(glm::vec3(0, 1, 0)),
    camera_right(glm::normalize(glm::cross(up, camera_direction))),
    m_cameraSensitivity(0.5)
{}


void Camera::MoveForward(const float delta_time)
{
    camera_position += camera_front * delta_time*m_cameraSensitivity;
}

void Camera::MoveBackward(const float delta_time)
{
    camera_position -= camera_front * delta_time*m_cameraSensitivity;

}

void Camera::MoveRight(const float delta_time)
{
    camera_position += glm::normalize(
        glm::cross(camera_front, camera_up))
        * delta_time*m_cameraSensitivity;
}

void Camera::MoveLeft(const float delta_time)
{
    camera_position -= glm::normalize(
        glm::cross(camera_front, camera_up))
        * delta_time*m_cameraSensitivity;
}

void Camera::MouseMoveFirstPerson(int xMove, int  yMove,float delta_time)
{
   
    xMove *= m_cameraSensitivity;
    yMove *= m_cameraSensitivity;
    yaw += xMove;
    pitch -= yMove;


    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}
