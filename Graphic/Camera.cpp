#include "stdafx.h"
#include "Camera.h"

#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

graphic::Camera::Camera()
	: moveSpeed(5), sensitivity(0.25), width(400), height(400), fov(45),
	nearestPoint(0.1), furthestPoint(100),
	mode(PERSPECTIVE)
{
	pos = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 graphic::Camera::getViewMatrix() const
{
	return (glm::lookAt(pos, front + pos, up));
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

void graphic::Camera::movement(graphic::Window &context, float delta)
{
	width = static_cast<float>(context.getWidth());
	height = static_cast<float>(context.getHeight());

	static glm::vec2 lastPos;
	glm::vec2 mousePos = context.getMousePos();
	float xpos = (mousePos.x - lastPos.x) * sensitivity;
	float ypos = (mousePos.y - lastPos.y) * sensitivity;

	if (context.checkMouse(GLFW_MOUSE_BUTTON_LEFT))
	{
		front += -xpos * glm::normalize(glm::cross(front, up)) * (sensitivity / 4)
			+ ypos * glm::normalize(up) * (sensitivity / 4);
	}
	else if (context.checkMouse(GLFW_MOUSE_BUTTON_MIDDLE))
	{
		pos -= ypos * glm::normalize(up) * sensitivity;
		pos -= xpos * glm::normalize(glm::cross(front, up)) * sensitivity;
	}
	else if (context.checkMouse(GLFW_MOUSE_BUTTON_RIGHT))
	{
		pos += (xpos - ypos) * glm::normalize(front) * sensitivity;
	}
	
	lastPos = mousePos;
}

void graphic::Camera::setFov(float value)
{
	fov = value;
}

void graphic::Camera::setNearestPoint(float value)
{
	nearestPoint = value;
}

void graphic::Camera::setFurthestPoint(float value)
{
	furthestPoint = value;
}

void graphic::Camera::setNewPosition(glm::mat4 transform)
{
	pos = transform[3];
	up = glm::vec4(0, 1, 0, 0) * transform;
	front = glm::vec4(0, 0, 1, 0) * transform;
}