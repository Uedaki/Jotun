#pragma once

#include "Material.h"

namespace collada
{
	class Lambert : public Material
	{
		std::pair<int, std::pair<glm::vec4, std::string>> emission;
		std::pair<int, std::pair<glm::vec4, std::string>> ambient;
		std::pair<int, std::pair<glm::vec4, std::string>> diffuse;
		std::pair<int, std::pair<glm::vec4, std::string>> reflective;
		std::pair<int, float> reflectivity;

	public:
		Lambert(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode);

		void setShader(graphic::ShaderGen &gen) override;
	};
}
