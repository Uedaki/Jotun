#include "stdafx.h"
#include "Collada.h"

#include <map>
#include <functional>
#include "GeometryInstance.h"
#include "LightInstance.h"
#include "Camera.h"

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
	const std::map<std::string, std::function<std::shared_ptr<ObjectInstance>(const xmlParser::Node &node, std::vector<std::shared_ptr<Light>> &lightContext)>> router = {
		{"instance_geometry", [this](const xmlParser::Node &node, std::vector<std::shared_ptr<Light>> &lightContext)
			{
				return (std::make_shared<GeometryInstance>(node, lightContext, this->getIdToNode()));
			}
		},
		{"instance_camera", [this](const xmlParser::Node &node, std::vector<std::shared_ptr<Light>> &lightContext)
			{
				return (std::make_shared<BasicObjectInstance<Camera>>(node, this->getIdToNode()));
			}
		}
	};

	std::vector<std::shared_ptr<Light>> lightContext;
	for (auto &node : visualScene.getChildren())
	{
		if (node.getTag() != "node")
			continue;
		for (auto &child : node.getChildren())
		{
			if (child.getTag() == "instance_light")
			{
				const std::string &url = child.getParameter("url").getString();
				LightInstance inst(idToNode.at(url.substr(1, url.length() - 1)));
				modifyInstance(node, inst);
				lightContext.push_back(inst.getLight());
				break;
			}
		}
	}

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
				object = router.at(child.getTag())(child, lightContext);
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