#pragma once

#include "ObjectInstance.h"
#include "Light.h"

namespace collada
{
	class LightInstance : public ObjectInstance
	{
		std::shared_ptr<Light> light;

	public:
		LightInstance(const xmlParser::Node &node);
		std::shared_ptr<Light> getLight();
		void instanciate(graphic::Scene &scene) override;
	};
}