#include "stdafx.h"

#include <fstream>
#include <sstream>
#include "Parser.h"

xmlParser::Parser::Parser()
	: first(0), end(-1)
{}

xmlParser::Parser::Parser(const std::string &filename)
	: first(0), end(-1)
{
	loadFile(filename);
}

void xmlParser::Parser::loadFile(const std::string &filename)
{
	std::ifstream file(filename);
	std::getline(file, content, '\0');
	createTree();
}

const std::string &xmlParser::Parser::getNextNode()
{
	if ((first = content.find_first_not_of(" \t\n", end + 1)) == std::string::npos)
	{
		currNode = "";
		residue = "";
		return (currNode);
	}

	const size_t start = first;
	if ((first = content.find_first_of('<', start)) != std::string::npos
		&& (end = content.find_first_of('>', first)) != std::string::npos)
	{
		currNode = content.substr(first + 1, end - first - 1);
		residue = content.substr(start, first - start);
	}
	else
		currNode = "";

	return (currNode);
}

void xmlParser::Parser::createTree()
{
	root = nullptr;
	while (!getNextNode().empty() && (currNode.back() == '?' || currNode.front() == '!'));
	if (currNode.empty())
		return;
	root = std::make_shared<Node>();
	fillNode(*root);
	if (currNode.back() != '/')
		parse(*root);
}

void xmlParser::Parser::parse(Node &father)
{
	while (getNextNode() != "/" + father.getTag() && !currNode.empty())
	{
		if (currNode.front() == '!')
			continue;
		Node &child = father.addChild();
		fillNode(child);
		if (currNode.back() != '/')
			parse(child);
	}

	if (!residue.empty())
	{
		father.setContent(residue);
	}
}

void xmlParser::Parser::fillNode(Node &node)
{
	std::string extract = currNode;
	if (currNode.back() == '/')
		extract = currNode.substr(0, currNode.length() - 1);
	std::istringstream stream(extract);
	
	stream >> extract;
	node.setTag(extract);
	while (stream.peek() != -1)
	{
		stream >> extract;

		size_t equalIdx;
		if ((equalIdx = extract.find_first_of("=")) == std::string::npos)
			continue;
		node.addParameter(extract.substr(0, equalIdx), extract.substr(equalIdx + 1, extract.length() - equalIdx - 1));
	}
}

const std::shared_ptr<xmlParser::Node> xmlParser::Parser::getRoot() const
{
	return (root);
}