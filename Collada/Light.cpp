#include "stdafx.h"
#include "Light.h"

#include <sstream>
glm::vec3 getColor(const xmlParser::Node &node)
{
	glm::vec3 color;
	std::string elem;
	std::istringstream stream(node.getContent().getString());

	for (uint8_t idx = 0; idx < 3; ++idx)
	{
		if (stream.peek() != -1)
		{
			stream >> elem;
			color[idx] = std::stof(elem);
		}
		else
			color[idx] = 1;
	}
	return (color);
}

float getFloat(const xmlParser::Node &node)
{
	return (node.getContent().getNumber<float>());
}

void collada::Light::setName(const std::string &newName)
{
	name = newName;
}

void collada::Light::setIntensity(float value)
{
	intensity = value;
}

void collada::Light::setTransform(glm::mat4 &newTransform)
{
	transform = newTransform;
}

collada::Ambient::Ambient(const xmlParser::Node &node)
{
	if (node.isNodeExist("color"))
	color = getColor(node["color"]);
}

void collada::Ambient::genLight(graphic::ShaderGen &gen)
{
	graphic::ShaderGen::AmbientLight light;
	light.color = color;
	light.intensity = intensity.getFloat();
	gen.addAmbientLight(light);
}

collada::Directional::Directional(const xmlParser::Node &node)
{
	color("color", node);
}

void collada::Directional::genLight(graphic::ShaderGen &gen)
{

}

collada::Spot::Spot(const xmlParser::Node &node)
	: color(1, 1, 1), constAttenuation(1), linearAttenuation(0), quadAttenuation(0),
	falloffAngle(10), falloffExp(45)
{
	if (node.isNodeExist("color"))
		color = getColor(node["color"]);

	if (node.isNodeExist("constant_attenuation"))
		constAttenuation = getFloat(node["constant_attenuation"]);

	if (node.isNodeExist("linear_attenuation"))
		linearAttenuation = getFloat(node["linear_attenuation"]);

	if (node.isNodeExist("quadratic_attenuation"))
		quadAttenuation = getFloat(node["quadratic_attenuation"]);

	if (node.isNodeExist("falloff_angle"))
		falloffAngle = getFloat(node["falloff_angle"]);

	if (node.isNodeExist("falloff_exponent"))
		falloffExp = getFloat(node["falloff_exponent"]);
}

void collada::Spot::genLight(graphic::ShaderGen &gen)
{
	gen.require("NORMAL", graphic::ShaderGen::FRAGMENT);
	graphic::ShaderGen::SpotLight light;
	light.name = name;
	light.position = transform[3];
	light.direction = glm::vec3(glm::vec4(0, 0, 1, 0) * transform) - light.position;
	light.angle = glm::cos(glm::radians(falloffAngle));
	light.intensity = intensity.getFloat();
	light.constant = constAttenuation;
	light.quadratic = quadAttenuation;
	light.linear = linearAttenuation;
	light.diffuse = color;
	light.ambient = color;
	light.specular = color;
	gen.addSpotLight(light);
}


collada::Point::Point(const xmlParser::Node &node)
	: color(1, 1, 1), constAttenuation(1), linearAttenuation(0), quadAttenuation(0)
{
	if (node.isNodeExist("color"))
		color = getColor(node["color"]);

	if (node.isNodeExist("constant_attenuation"))
	constAttenuation = getFloat(node["constant_attenuation"]);

	if (node.isNodeExist("linear_attenuation"))
	linearAttenuation = getFloat(node["linear_attenuation"]);

	if (node.isNodeExist("quadratic_attenuation"))
	quadAttenuation = getFloat(node["quadratic_attenuation"]);
}

void collada::Point::genLight(graphic::ShaderGen &gen)
{
	gen.require("NORMAL", graphic::ShaderGen::FRAGMENT);
	graphic::ShaderGen::PointLight light;
	light.name = name;
	light.position = transform[3];
	light.intensity = intensity.getFloat();
	light.constant = constAttenuation;
	light.quadratic = quadAttenuation;
	light.linear = linearAttenuation;
	light.diffuse = color;
	light.ambient = color;
	light.specular = color;
	gen.addPointLight(light);
}