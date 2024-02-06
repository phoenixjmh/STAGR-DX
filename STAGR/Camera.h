#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera
{
public:
	glm::vec3 camera_position;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	glm::vec3 camera_target;
	glm::vec3 camera_direction;
	glm::vec3 up;
	glm::vec3 camera_right;
	

	//temporary fix ew gross
	float MouseLastX = 0;
	float MouseLastY = 0;
	float yaw=-90.f, pitch;
	bool MouseFirst=true;

	Camera();
		

	void MoveForward(const float camera_speed);

	void MoveBackward(const float camera_speed);

	void MoveRight(const float camera_speed);
	
	void MoveLeft(const float camera_speed);
	void MouseMoveFirstPerson(int xPos, int yPos,float time_step);
private:
	float m_cameraSensitivity;
};
