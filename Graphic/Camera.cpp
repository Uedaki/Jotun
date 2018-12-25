#include "stdafx.h"
#include "Camera.h"

#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

graphic::Camera::Camera()
	: moveSpeed(1), width(400), height(400), fov(45),
	nearestPoint(0.1), furthestPoint(100),
	mode(PERSPECTIVE)
{
	pos = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 graphic::Camera::getViewMatrix() const
{
	return (glm::lookAt(pos, front, up));
}

glm::mat4 graphic::Camera::getProjectionMatrix() const
{
	if (mode == CameraMode::ORTHO)
	{
		return (glm::ortho(0.0f, width, 0.0f, height,
			nearestPoint, furthestPoint));
	}
	else
	{
		return (glm::perspective(fov, width / height,
			nearestPoint, furthestPoint));
	}
}