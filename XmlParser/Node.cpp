#include "stdafx.h"
#include "Node.h"

xmlParser::Node::Node()
{}

void xmlParser::Node::setTag(const std::string &newTag)
{
	tag = newTag;
}

const std::string &xmlParser::Node::getTag() const
{
	return (tag);
}

void xmlParser::Node::addParameter(const std::string &key, const std::string &value)
{
	parameters[key] = value;
}

bool xmlParser::Node::isParameterExist(const std::string &key) const
{
	return (parameters.find(key) != parameters.end());
}

const xmlParser::Value &xmlParser::Node::getParameter(const std::string &key) const
{
	return (parameters.at(key));
}

const std::map<std::string, xmlParser::Value> &xmlParser::Node::getParameters() const
{
	return (parameters);
}

void xmlParser::Node::setContent(const std::string &newContent)
{
	content = newContent;
}

const xmlParser::Value &xmlParser::Node::getContent() const
{
	return (content);
}

xmlParser::Node &xmlParser::Node::addChild()
{
	children.emplace_back();
	return (children.back());
}

const std::vector<xmlParser::Node> &xmlParser::Node::getChildren() const
{
	return (children);
}

bool xmlParser::Node::isNodeExist(const std::string &tag) const
{
	for (auto &node : children)
	{
		if (node.tag == tag)
		{
			return (true);
		}
	}
	return (false);
}

const xmlParser::Node &xmlParser::Node::operator[](const std::string &tag) const
{
	for (auto &node : children)
	{
		if (node.tag == tag)
		{
			return (node);
		}
	}
	throw std::runtime_error("[xmlParser::Node] Trying to reach an unknown node");
}