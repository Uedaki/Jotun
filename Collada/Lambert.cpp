#include "stdafx.h"
#include "Lambert.h"

collada::Lambert::Lambert(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
{
	fillComponent("emission", emission, node, idToNode);
	fillComponent("ambient", ambient, node, idToNode);
	fillComponent("diffuse", diffuse, node, idToNode);
	fillComponent("reflective", reflective, node, idToNode);

	if (node.isNodeExist("reflectivity") && node["reflectivity"].isNodeExist("float"))
	{
		reflectivity.second = node["reflectivity"]["float"].getContent().getNumber<float>();
	}
	else
		reflectivity.first = -1;
}

void collada::Lambert::setShader(graphic::ShaderGen &gen)
{
	Material::setShader(gen);

	if (diffuse.first == -1)
	{
		gen.setObjectColor("vec4(0.4f, 0.1f, 0.2f, 1.0f)");
	}
	else if (diffuse.first == 0)
	{
		gen.setObjectColor("vec4(" + std::to_string(diffuse.second.first[0])
			+ ", " + std::to_string(diffuse.second.first[1])
			+ ", " + std::to_string(diffuse.second.first[2])
			+ ", " + std::to_string(diffuse.second.first[3]) + ")");
	}
	else if (diffuse.first == 1)
	{
		gen.require("TEXCOORD", graphic::ShaderGen::FRAGMENT);
		gen.addFragUniform("myTexture", "sampler2D");
		gen.setObjectColor("texture(myTexture, TEXCOORD)");
	}
}