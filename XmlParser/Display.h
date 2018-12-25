#pragma once

#include "Node.h"

#ifdef XMLPARSER_EXPORTS  
#define XML_PARSER_API __declspec(dllexport)   
#else  
#define XML_PARSER_API __declspec(dllimport)   
#endif

namespace xmlParser
{
	class Display
	{
	public:
		XML_PARSER_API static void nodeTree(const Node &root);
		XML_PARSER_API static void complexNodeTree(const Node &root);
	};
}