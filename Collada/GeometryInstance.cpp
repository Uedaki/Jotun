#include "stdafx.h"
#include "GeometryInstance.h"

collada::GeometryInstance::GeometryInstance(const xmlParser::Node &node, std::vector<std::shared_ptr<Light>> &lightContext, const std::map<std::string, const xmlParser::Node &> &idToNode)
{
	const std::string &url = node.getParameter("url").getString();
	geometry = Geometry::define(idToNode.at(url.substr(1, url.length() - 1)));

	if (node.isNodeExist("bind_material"))
	{
		loadMaterials(node["bind_material"]["technique_common"], lightContext, idToNode);
		geometry->setMaterial(materials);
	}

}

void collada::GeometryInstance::loadMaterials(const xmlParser::Node &node, std::vector<std::shared_ptr<Light>> &lightContext, const std::map<std::string, const xmlParser::Node &> &idToNode)
{
	for (auto &material : node.getChildren())
	{
		if (material.getTag() != "instance_material")
			continue;
		const std::string &symbol = material.getParameter("symbol").getString();
		const std::string &target = material.getParameter("target").getString();

		materials.emplace(symbol, Material::instanciate(idToNode.at(target.substr(1, target.length() - 1)), idToNode));
		materials.at(symbol)->setLightContext(lightContext);
	}
}

void collada::GeometryInstance::instanciate(graphic::Scene &scene)
{
	geometry->instanciate(scene, transform);
}