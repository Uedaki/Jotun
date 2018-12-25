#pragma once

#include <glm/glm.hpp>
#include <Graphic/Scene.h>
#include <XmlParser/Node.h>

namespace collada
{
	class ObjectInstance
	{
	protected:
		glm::mat4 transform;

	public:
		ObjectInstance();
		virtual ~ObjectInstance() = default;

		virtual void lookAt(const xmlParser::Node &node);
		virtual void matrix(const xmlParser::Node &node);
		virtual void rotate(const xmlParser::Node &node);
		virtual void scale(const xmlParser::Node &node);
		virtual void skew(const xmlParser::Node &node);
		virtual void translate(const xmlParser::Node &node);

		virtual void instanciate(graphic::Scene &scene) = 0;
	};
}