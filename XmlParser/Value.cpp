#include "stdafx.h"

#include "Value.h"

xmlParser::Value::Value()
{}

xmlParser::Value &xmlParser::Value::operator=(const Value &ref)
{
	value = ref.value;
	return (*this);
}

xmlParser::Value &xmlParser::Value::operator=(const std::string &content)
{
	if (content.front() == '"' && content.back() == '"')
		value = content.substr(1, content.length() - 2);
	else
		value = content;
	return (*this);
}

bool xmlParser::Value::isNumber() const
{
	size_t dot;
	size_t start = 0;
	if (value.front() == '+' || value.front() == '-')
		start = 1;
	if ((dot = value.find_first_not_of("0123456789", start)) == std::string::npos)
		return (true);
	else if (value[dot] == '.' && value.find_first_not_of("0123456789", dot + 1) == std::string::npos)
		return (true);
	else
		return (false);
}

const std::string &xmlParser::Value::getString() const
{
	return (value);
}