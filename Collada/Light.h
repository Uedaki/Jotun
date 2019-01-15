#pragma once

#include <Graphic/ShaderGen.h>
#include "Value.h"

namespace collada
{
	class Light
	{
	protected:
		std::string name;
		glm::mat4 transform;
		FloatValue intensity;

	public:
		void setName(const std::string &newName);
		void setIntensity(float intensity);
		void setTransform(glm::mat4 &newTransform);
		virtual void genLight(graphic::ShaderGen &gen) = 0;
	};

	class Ambient : public Light
	{
		glm::vec3 color;

	public:
		Ambient(const xmlParser::Node &node);
		virtual void genLight(graphic::ShaderGen &gen);
	};

	class Directional : public Light
	{
		ColorValue color;

	public:
		Directional(const xmlParser::Node &node);
		virtual void genLight(graphic::ShaderGen &gen);
	};

	class Spot : public Light
	{
		glm::vec3 color;
		float constAttenuation;
		float linearAttenuation;
		float quadAttenuation;
		float falloffAngle;
		float falloffExp;

	public:
		Spot(const xmlParser::Node &node);
		virtual void genLight(graphic::ShaderGen &gen);
	};

	class Point : public Light
	{
		glm::vec3 color;
		float constAttenuation;
		float linearAttenuation;
		float quadAttenuation;

	public:
		Point(const xmlParser::Node &node);
		virtual void genLight(graphic::ShaderGen &gen);
	};
}