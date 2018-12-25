#include "stdafx.h"
#include "GeometryInstance.h"

collada::GeometryInstance::GeometryInstance(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode)
{
	const std::string &url = node.getParameter("url").getString();
	geometry = Geometry::define(idToNode.at(url.substr(1, url.length() - 1)));

	if (node.isNodeExist("bind_material"))
		loadMaterials(node["bind_material"]["technique_common"]);
}

void collada::GeometryInstance::loadMaterials(const xmlParser::Node &node)
{

}

void collada::GeometryInstance::instanciate(graphic::Scene &scene)
{
	geometry->instanciate(scene, transform);
}