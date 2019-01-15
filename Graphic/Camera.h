#pragma once

#include <glm/glm.hpp>
#include "Window.h"

#ifdef GRAPHIC_EXPORTS  
#define GRAPHIC_API __declspec(dllexport)   
#else  
#define GRAPHIC_API __declspec(dllimport)   
#endif

namespace graphic
{
	enum CameraMode
	{
		ORTHO,
		PERSPECTIVE
	};

	struct Camera
	{
		float moveSpeed;
		float sensitivity;
		float width;
		float height;
		float fov;
		float nearestPoint;
		float furthestPoint;
		CameraMode mode;
		glm::vec3 pos;
		glm::vec3 front;
		glm::vec3 up;

	public:
		GRAPHIC_API Camera();

		GRAPHIC_API glm::mat4 getViewMatrix() const;
		GRAPHIC_API glm::mat4 getProjectionMatrix() const;

		GRAPHIC_API void movement(Window &context, float delta);

		GRAPHIC_API void setFov(float value);
		GRAPHIC_API void setNearestPoint(float value);
		GRAPHIC_API void setFurthestPoint(float value);

		GRAPHIC_API void setNewPosition(glm::mat4 transform);
	};
}