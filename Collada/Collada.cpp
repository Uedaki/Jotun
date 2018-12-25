#include "stdafx.h"
#include "Collada.h"

#include <map>
#include <functional>
#include "GeometryInstance.h"

void collada::Collada::loadFile(const std::string &file)
{
	xmlParser::Parser parser(file);
	root = parser.getRoot();
	mapNodeById(*root);
}

void collada::Collada::mapNodeById(const xmlParser::Node &node)
{
	if (node.isParameterExist("id"))
		idToNode.emplace(node.getParameter("id").getString(), node);

	for (auto &child : node.getChildren())
	{
		mapNodeById(child);
	}
}

std::shared_ptr<graphic::Scene> collada::Collada::createScene()
{
	scene = std::make_shared<graphic::Scene>();

	const xmlParser::Node &instVisualScene = (*root)["scene"]["instance_visual_scene"];
	const std::string &url = instVisualScene.getParameter("url").getString();
	const xmlParser::Node &visualScene = idToNode.at(url.substr(1, url.length() - 1));

	createVisualScene(visualScene);

	return (scene);
}

void collada::Collada::createVisualScene(const xmlParser::Node &visualScene)
{
	const std::map<std::string, std::function<std::shared_ptr<ObjectInstance>(const xmlParser::Node &node)>> router = {
		{"instance_geometry", [this](const xmlParser::Node &node)
			{
				return (std::make_shared<GeometryInstance>(node, this->getIdToNode()));
			}
		}
	};

	for (auto &node : visualScene.getChildren())
	{
		std::shared_ptr<ObjectInstance> object = nullptr;
		if (node.getTag() != "node")
			continue;
		for (auto &child : node.getChildren())
		{
			if (child.getTag().find_first_of("instance") == 0
				&& router.find(child.getTag()) != router.cend())
			{
				object = router.at(child.getTag())(child);
				break;
			}
		}

		if (!object)
			continue;
		modifyInstance(node, *object);
		object->instanciate(*scene);
	}
}

void collada::Collada::modifyInstance(const xmlParser::Node &node, ObjectInstance &instance)
{
	const std::map<std::string, std::function<void(const xmlParser::Node &node, ObjectInstance &)>> modifier = {
		{"lookat", [](const xmlParser::Node &node, ObjectInstance &instance)
			{
				instance.lookAt(node);
			}
		},
		{"matrix", [](const xmlParser::Node &node, ObjectInstance &instance)
			{
				instance.matrix(node);
			}
		},
		{"rotate", [](const xmlParser::Node &node, ObjectInstance &instance)
			{
				instance.rotate(node);
			}
		},
		{"scale", [](const xmlParser::Node &node, ObjectInstance &instance)
			{
				instance.scale(node);
			}
		},
		{"skew", [](const xmlParser::Node &node, ObjectInstance &instance)
			{
				instance.skew(node);
			}
		},
		{"translate", [](const xmlParser::Node &node, ObjectInstance &instance)
			{
				instance.translate(node);
			}
		},
	};

	for (auto &child : node.getChildren())
	{
		if (modifier.find(child.getTag()) != modifier.cend())
			modifier.at(child.getTag())(child, instance);
	}
}

const std::map<std::string, const xmlParser::Node &> &collada::Collada::getIdToNode() const
{
	return (idToNode);
}