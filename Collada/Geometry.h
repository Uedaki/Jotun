#pragma once

#include <memory>
#include <Graphic/Scene.h>
#include <XmlParser/Node.h>
#include "Material.h"

#ifdef COLLADA_EXPORTS  
#define COLLADA_API __declspec(dllexport)   
#else  
#define COLLADA_API __declspec(dllimport)   
#endif

namespace collada
{
	class Geometry
	{
		

	public:
		static std::shared_ptr<Geometry> define(const xmlParser::Node &node);
		virtual ~Geometry() = default;

		virtual void setMaterial(const std::map<std::string, std::shared_ptr<Material>> &materials) = 0;

		virtual void instanciate(graphic::Scene &scene, glm::mat4 transform) = 0;
	};
}