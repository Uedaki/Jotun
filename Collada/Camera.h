#pragma once

#include <XmlParser/Node.h>
#include <Graphic/Scene.h>
#include <glm/glm.hpp>

#ifdef COLLADA_EXPORTS  
#define COLLADA_API __declspec(dllexport)   
#else  
#define COLLADA_API __declspec(dllimport)   
#endif

namespace collada
{
	class Camera
	{
		graphic::CameraMode mode;
		float width;
		float height;
		float fov;
		float nearestPoint;
		float furthestPoint;

		void perspective(const xmlParser::Node &node);
		void orthographic(const xmlParser::Node &node);
	public:
		COLLADA_API static std::shared_ptr<Camera> define(const xmlParser::Node &node);

		COLLADA_API Camera(const xmlParser::Node &node);

		COLLADA_API void instanciate(graphic::Scene &scene, glm::mat4 transform);
	};
}