#include "Parser.h"
#include "Display.h"

int main()
{
	xml::Parser parser("xmlTest.txt");
	auto root = parser.getRoot();
	xml::Display::complexNodeTree(*root);
	getchar();
}