#include "stdafx.h"
#include "Geometry.h"

#include <map>
#include <functional>
#include "Mesh.h"

std::shared_ptr<collada::Geometry> collada::Geometry::define(const xmlParser::Node &node)
{
	const std::map<std::string, std::function<std::shared_ptr<Geometry>(const xmlParser::Node &node)>> router =
	{
		{"mesh", [](const xmlParser::Node &node)
		{
			return (std::make_shared<Mesh>(node));
		}}
	};

	for (auto &child : node.getChildren())
	{
		if (child.getTag() != "asset" && child.getTag() != "extra" && router.find(child.getTag()) != router.cend())
		{
			return (router.at(child.getTag())(child));
		}
	}
	return (nullptr);
}