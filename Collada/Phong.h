#pragma once

#include "Material.h"

namespace collada
{
	class Phong : public Material
	{
		std::pair<int, std::pair<glm::vec4, std::string>> emission;
		std::pair<int, std::pair<glm::vec4, std::string>> ambient;
		std::pair<int, std::pair<glm::vec4, std::string>> diffuse;
		std::pair<int, std::pair<glm::vec4, std::string>> specular;
		float shininess;
		std::pair<int, std::pair<glm::vec4, std::string>> reflective;
		float reflectivity;

	public:
		Phong(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode);

		void setShader(graphic::ShaderGen &gen) override;
	};
}