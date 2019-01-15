#pragma once

#include <glm/glm.hpp>
#include <XmlParser/Node.h>
#include <Graphic/ShaderGen.h>
#include "Light.h"

namespace collada
{
	class Material
	{
	protected:
		std::string texture;
		std::vector<std::shared_ptr<Light>> *lightContext;

		glm::vec4 retreiveColor(const xmlParser::Node &node);
		void retreiveTexture(const xmlParser::Node &node, std::pair<int, std::pair<glm::vec4, std::string>> &comp,const std::map<std::string, const xmlParser::Node &> &idToNode);
		void fillComponent(const std::string &name, std::pair<int, std::pair<glm::vec4, std::string>> &comp, const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode);

	public:
		static std::shared_ptr<Material> instanciate(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode);
		virtual void getTextures(std::string &text);
		virtual void setShader(graphic::ShaderGen &gen);
		void setLightContext(std::vector<std::shared_ptr<Light>> &lightContext);
	};
}