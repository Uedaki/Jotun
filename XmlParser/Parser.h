#pragma once

#include <memory>
#include <string>
#include "Node.h"

#ifdef XMLPARSER_EXPORTS  
#define XML_PARSER_API __declspec(dllexport)   
#else  
#define XML_PARSER_API __declspec(dllimport)   
#endif

namespace xmlParser
{
	class Parser
	{
		std::string content;
		std::string currNode;
		std::string residue;
		size_t first;
		size_t end;

		std::shared_ptr<Node> root;

		const std::string &getNextNode();
		void createTree();
		void parse(Node &father);
		void fillNode(Node &node);
	public:
		XML_PARSER_API Parser();
		XML_PARSER_API Parser(const std::string &file);
		~Parser() = default;

		XML_PARSER_API void loadFile(const std::string &file);
		XML_PARSER_API const std::shared_ptr<Node> getRoot() const;
	};
}