#include "stdafx.h"
#include "Phong.h"

collada::Phong::Phong(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
	: shininess(0)
{
	fillComponent("emission", emission, node, idToNode);
	fillComponent("ambient", ambient, node, idToNode);
	fillComponent("diffuse", diffuse, node, idToNode);
	fillComponent("specular", specular, node, idToNode);

	if (node.isNodeExist("shininess") && node["shininess"].isNodeExist("float"))
	{
		shininess = node["shininess"]["float"].getContent().getNumber<float>();
	}

	fillComponent("reflective", reflective, node, idToNode);
}

void collada::Phong::setShader(graphic::ShaderGen &gen)
{
	Material::setShader(gen);

	gen.setShininess(shininess);

	if (diffuse.first == -1)
	{
		gen.setObjectColor("vec4(0.4f, 0.4f, 0.4f, 1.0f)");
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

	if (specular.first == 0)
	{
		gen.setObjectSpecular("vec4(" + std::to_string(specular.second.first[0])
			+ ", " + std::to_string(specular.second.first[1])
			+ ", " + std::to_string(specular.second.first[2])
			+ ", " + std::to_string(specular.second.first[3]) + ")");
	}
	else if (specular.first == 1)
	{
		gen.require("TEXCOORD", graphic::ShaderGen::FRAGMENT);
		gen.addFragUniform("myTexture", "sampler2D");
		gen.setObjectSpecular("texture(myTexture, TEXCOORD)");
	}
}