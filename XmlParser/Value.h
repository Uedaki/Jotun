#pragma once

#include <string>

#ifdef XMLPARSER_EXPORTS  
#define XML_PARSER_API __declspec(dllexport)   
#else  
#define XML_PARSER_API __declspec(dllimport)   
#endif

namespace xmlParser
{
	class Value
	{
		std::string value;

	public:
		Value();
		~Value() = default;

		Value &operator=(const Value &ref);
		Value &operator=(const std::string &content);

		XML_PARSER_API bool isNumber() const;

		template <typename T>
		T getNumber() const
		{
			return (static_cast<T>(std::stof(value)));
		}

		XML_PARSER_API const std::string &getString() const;
	};
}