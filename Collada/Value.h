#pragma once

#include <glm/glm.hpp>
#include <XmlParser/Node.h>

namespace collada
{
	class ColorValue
	{
		bool bHasColor;
		glm::vec4 color;

	public:
		ColorValue();
		void fillColor(const xmlParser::Node &node);
		glm::vec4 getColor() const;
		bool hasColor() const;

		void operator()(const std::string &name, const xmlParser::Node &node);
	};

	class FloatValue
	{
		bool bHasFloat;
		float value;

	public:
		FloatValue();
		void fillFloat(const xmlParser::Node &node);
		float getFloat() const;
		bool hasFloat() const;

		void operator()(const std::string &name, const xmlParser::Node &node);
		FloatValue &operator=(float value);
	};
}