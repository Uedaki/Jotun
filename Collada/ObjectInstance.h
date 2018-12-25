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

	template <typename Object>
	class BasicObjectInstance : public ObjectInstance
	{
		std::shared_ptr<Object> object;

	public:
		BasicObjectInstance(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
		{
			const std::string &url = node.getParameter("url").getString();
			object = Object::define(idToNode.at(url.substr(1, url.length() - 1)));
		}

		void instanciate(graphic::Scene &scene) override
		{
			object->instanciate(scene, transform);
		}
	};
}