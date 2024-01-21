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

	Camera()
		:
		camera_position(glm::vec3(0.0, 0.0, 30.0f)),
		camera_front(glm::vec3(0, 0, -1)),
		camera_up(glm::vec3(0, 1, 0)), camera_target(glm::vec3(0, 0, -1)),
		camera_direction(glm::normalize(camera_position - camera_target)),
		up(glm::vec3(0, 1, 0)),
		camera_right(glm::normalize(glm::cross(up, camera_direction)))
	{}
};
