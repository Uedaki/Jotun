#include "stdafx.h"
#include "ObjectInstance.h"

#include <sstream>

collada::ObjectInstance::ObjectInstance()
	: transform(glm::mat4(1))
{}

void collada::ObjectInstance::lookAt(const xmlParser::Node &node)
{

}

void collada::ObjectInstance::matrix(const xmlParser::Node &node)
{
	std::string elem;
	std::istringstream stream(node.getContent().getString());
	for (uint16_t idx = 0; idx < 16; ++idx)
	{
		stream >> elem;
		transform[idx / 4][idx % 4] = std::stof(elem);
	}
}

void collada::ObjectInstance::rotate(const xmlParser::Node &node)
{

}

void collada::ObjectInstance::scale(const xmlParser::Node &node)
{

}

void collada::ObjectInstance::skew(const xmlParser::Node &node)
{

}

void collada::ObjectInstance::translate(const xmlParser::Node &node)
{

}