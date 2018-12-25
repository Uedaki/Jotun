#include "stdafx.h"
#include "Camera.h"

#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

graphic::Camera::Camera()
	: moveSpeed(5), width(400), height(400), fov(45),
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
		return (glm::perspective(fov, 16.0f / 9.0f,
			nearestPoint, furthestPoint));
	}
}

void graphic::Camera::movement(graphic::Window &context, float delta)
{
	width = static_cast<float>(context.getWidth());
	height = static_cast<float>(context.getHeight());

	if (context.isKeyPressed(GLFW_KEY_W) == GLFW_PRESS)
		pos -= moveSpeed * glm::normalize(pos + front) * delta;
	if (context.isKeyPressed(GLFW_KEY_S) == GLFW_PRESS)
		pos += moveSpeed * glm::normalize(pos + front) * delta;
	if (context.isKeyPressed(GLFW_KEY_A) == GLFW_PRESS)
		pos -= glm::normalize(glm::cross(pos + front, up)) * moveSpeed * delta;
	if (context.isKeyPressed(GLFW_KEY_D) == GLFW_PRESS)
		pos += glm::normalize(glm::cross(pos + front, up)) * moveSpeed * delta;
}