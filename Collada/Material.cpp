#include "stdafx.h"
#include "Material.h"

#include <sstream>
#include <functional>
#include "Lambert.h"
#include "Phong.h"

std::shared_ptr<collada::Material> collada::Material::instanciate(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
{
	const std::vector<std::pair<std::string, std::function<std::shared_ptr<Material>(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)>>> router = {
		{"lambert", [](const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
			{
				return (std::make_shared<Lambert>(node, idToNode));
			}
		},
		{"phong", [](const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
			{
				return (std::make_shared<Phong>(node, idToNode));
			}
		},
		{"blinn", [](const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
			{
				return (std::make_shared<Phong>(node, idToNode));
			}
		}
	};

	if (node.isNodeExist("instance_effect"))
	{
		const std::string &url = node["instance_effect"].getParameter("url").getString();
		const xmlParser::Node &effect = idToNode.at(url.substr(1, url.length() - 1));
		const xmlParser::Node &technique = effect["profile_COMMON"]["technique"];

		for (auto &route : router)
		{
			if (technique.isNodeExist(route.first))
			{
				return (route.second(technique[route.first], idToNode));
			}
		}
	}

	return (nullptr);
}

glm::vec4 collada::Material::retreiveColor(const xmlParser::Node &node)
{
	glm::vec4 color;
	std::string elem;
	std::istringstream stream(node.getContent().getString());

	for (uint8_t idx = 0; idx < 4; ++idx)
	{
		stream >> elem;
		color[idx] = std::stof(elem);
	}
	return (color);
}

void collada::Material::retreiveTexture(const xmlParser::Node &node, std::pair<int, std::pair<glm::vec4, std::string>> &comp, const std::map<std::string, const xmlParser::Node &> &idToNode)
{
	const std::string &url = node.getParameter("texture").getString();
	const xmlParser::Node &image = idToNode.at(url);
	comp.second.second = image["init_from"].getContent().getString();
	if (comp.second.second.find_first_of("file://") == 0)
		comp.second.second = comp.second.second.substr(sizeof("file://") - 1, comp.second.second.length() - sizeof("file://") + 1);
	texture = comp.second.second;
}

void collada::Material::fillComponent(const std::string &name, std::pair<int, std::pair<glm::vec4, std::string>> &comp,
	const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
{
	if (node.isNodeExist(name))
	{
		const xmlParser::Node &compNode = node[name];
		if (compNode.isNodeExist("color"))
		{
			comp.second.first = retreiveColor(compNode["color"]);
			comp.first = 0;
		}
		else if (node.isNodeExist(name))
		{
			retreiveTexture(compNode["texture"], comp, idToNode);
			comp.first = 1;
		}
		else
			comp.first = -1;
	}
	else
	{
		comp.first = -1;
	}
}

void collada::Material::getTextures(std::string &text)
{
	text = texture;
}

void collada::Material::setLightContext(std::vector<std::shared_ptr<Light>> &lightContext)
{
	this->lightContext = &lightContext;
}

void collada::Material::setShader(graphic::ShaderGen &gen)
{
	for (auto &light : *lightContext)
	{
		light->genLight(gen);
	}
}