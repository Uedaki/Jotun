#pragma once

#include "ObjectInstance.h"
#include "Geometry.h"

namespace collada
{
	class GeometryInstance : public ObjectInstance
	{
		std::shared_ptr<Geometry> geometry;

		void loadMaterials(const xmlParser::Node &node);
	public:
		GeometryInstance(const xmlParser::Node &node, const std::map<std::string, const xmlParser::Node &> &idToNode);
		void instanciate(graphic::Scene &scene) override;
	};
}