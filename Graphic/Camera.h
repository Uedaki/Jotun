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

	class Camera
	{
		float moveSpeed;
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
	};
}