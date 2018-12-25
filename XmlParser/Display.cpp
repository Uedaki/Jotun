#include "stdafx.h"
#include "Display.h"

#include <iostream>

static void displayNodeTree(const xmlParser::Node &father, const std::string &indentation)
{
	std::cout << indentation << father.getTag() << "\n";

	std::string newIndentation;
	if (indentation.empty())
		newIndentation = "|-";
	else
		newIndentation = "  " + indentation;

	for (auto &child : father.getChildren())
	{
		displayNodeTree(child, newIndentation);
	}
}

void xmlParser::Display::nodeTree(const Node &root)
{
	displayNodeTree(root, "");
	std::cout << std::flush;
}

static void displayComplexNodeTree(const xmlParser::Node &father, const std::string &indentation)
{
	std::cout << indentation << father.getTag();

	std::map<std::string, xmlParser::Value> parameters = father.getParameters();
	if (parameters.size() != 0)
	{
		bool isFirst = true;
		std::cout << " {";
		for (auto &param : parameters)
		{
			if (isFirst)
				isFirst = false;
			else
				std::cout << ", ";
			std::cout << param.first << "=" << param.second.getString();
		}
		std::cout << "}";
	}

	if (!father.getContent().getString().empty())
		std::cout << " [" << father.getContent().getString() << "]";

	std::cout << "\n";
	std::string newIndentation;
	if (indentation.empty())
		newIndentation = "|-";
	else
		newIndentation = "  " + indentation;

	for (auto &child : father.getChildren())
	{
		displayComplexNodeTree(child, newIndentation);
	}
}

void xmlParser::Display::complexNodeTree(const Node &root)
{
	displayComplexNodeTree(root, "");
	std::cout << std::flush;
}