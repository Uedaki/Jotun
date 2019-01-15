#include "stdafx.h"
#include "LightInstance.h"

#include <functional>

collada::LightInstance::LightInstance(const xmlParser::Node &node)
{
	const xmlParser::Node &common = node["technique_common"];
	const xmlParser::Node &technique = node["technique"];
	const std::map<std::string, std::function<std::shared_ptr<Light>(const xmlParser::Node &node)>> router = {
		{"ambient", [](const xmlParser::Node &node)
			{
				return (std::make_shared<Ambient>(node));
			}
		},
		{"directional", [](const xmlParser::Node &node)
			{
				return (std::make_shared<Directional>(node));
			}
		},
		{"spot", [](const xmlParser::Node &node)
			{
				return (std::make_shared<Spot>(node));
			}
		},
		{"point", [](const xmlParser::Node &node)
			{
				return (std::make_shared<Point>(node));
			}
		}
	};

	light = router.at(common.getChildren()[0].getTag())(common.getChildren()[0]);

	light->setName(node.getParameter("id").getString());

	if (technique.isNodeExist("intensity"))
		light->setIntensity(technique["intensity"].getContent().getNumber<float>());
}

std::shared_ptr<collada::Light> collada::LightInstance::getLight()
{
	light->setTransform(transform);
	return (light);
}

void collada::LightInstance::instanciate(graphic::Scene &scene)
{}