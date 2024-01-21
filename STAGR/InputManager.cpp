#include "InputManager.h"
#include "Renderer.h"
void InputManager::OnKeyPressed(UINT8 key)
{
    const float cameraSpeed = 0.5;
    Camera* CAMERA = &Renderer::Get().GetCamera();
    if (key == VK_SPACE)
    {
        CAMERA->camera_position -= glm::normalize(
            glm::cross(CAMERA->camera_front, CAMERA->camera_up))
            * cameraSpeed;
    }
}
