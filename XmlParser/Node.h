#pragma once

#include <map>
#include <vector>
#include <string>
#include "Value.h"

#ifdef XMLPARSER_EXPORTS  
#define XML_PARSER_API __declspec(dllexport)   
#else  
#define XML_PARSER_API __declspec(dllimport)   
#endif

namespace xmlParser
{
	class Node
	{
	private:
		std::string tag;
		Value content;
		std::map<std::string, Value> parameters;
		std::vector<Node> children;

	public:
		Node();
		~Node() = default;

		void setTag(const std::string &newTag);
		XML_PARSER_API const std::string &getTag() const;

		void addParameter(const std::string &key, const std::string &value);
		XML_PARSER_API bool isParameterExist(const std::string &key) const;
		XML_PARSER_API const xmlParser::Value &getParameter(const std::string &key) const;
		XML_PARSER_API const std::map<std::string, Value> &getParameters() const;

		void setContent(const std::string &newContent);
		XML_PARSER_API const Value &getContent() const;
		
		Node &addChild();
		XML_PARSER_API const std::vector<Node> &getChildren() const;

		XML_PARSER_API bool isNodeExist(const std::string &tag) const;
		XML_PARSER_API const Node &operator[](const std::string &tag) const;
	};
}