#pragma once

#include "ObjectInstance.h"
#include "Light.h"
#include "Geometry.h"
#include "Material.h"

namespace collada
{
	class GeometryInstance : public ObjectInstance
	{
		std::shared_ptr<Geometry> geometry;
		std::map<std::string, std::shared_ptr<Material>> materials;

		void loadMaterials(const xmlParser::Node &node, std::vector<std::shared_ptr<Light>> &lightContext, const std::map<std::string, const xmlParser::Node &> &idToNode);
	public:
		GeometryInstance(const xmlParser::Node &node, std::vector<std::shared_ptr<Light>> &lightContext, const std::map<std::string, const xmlParser::Node &> &idToNode);
		void instanciate(graphic::Scene &scene) override;
	};
}