#include "stdafx.h"
#include "Value.h"

#include <sstream>

collada::ColorValue::ColorValue()
	: bHasColor(false)
{}

void collada::ColorValue::fillColor(const xmlParser::Node &node)
{
	std::string elem;
	std::istringstream stream(node.getContent().getString());

	for (uint8_t idx = 0; idx < 4; ++idx)
	{
		if (stream.peek() != -1)
		{
			stream >> elem;
			color[idx] = std::stof(elem);
		}
		else
			color[idx] = 1;
	}
 	bHasColor = true;
}

glm::vec4 collada::ColorValue::getColor() const
{
	return (color);
}

bool collada::ColorValue::hasColor() const
{
	return (bHasColor);
}

void collada::ColorValue::operator()(const std::string &name, const xmlParser::Node &node)
{
	if (node.isNodeExist(name))
		fillColor(node[name]);
}

collada::FloatValue::FloatValue()
	: bHasFloat(false)
{}

void collada::FloatValue::fillFloat(const xmlParser::Node &node)
{
	glm::vec4 color;
	std::string elem;
	value = node.getContent().getNumber<float>();
	bHasFloat = true;
}

float collada::FloatValue::getFloat() const
{
	return (value);
}

bool collada::FloatValue::hasFloat() const
{
	return (bHasFloat);
}

void collada::FloatValue::operator()(const std::string &name, const xmlParser::Node &node)
{
	if (node.isNodeExist(name))
		fillFloat(node[name]);
}

collada::FloatValue &collada::FloatValue::operator=(float value)
{
	this->value = value;
	return (*this);
}