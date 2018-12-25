#pragma once

#include <memory>
#include <XmlParser/Parser.h>
#include <Graphic/Scene.h>
#include "ObjectInstance.h"

#ifdef COLLADA_EXPORTS  
#define COLLADA_API __declspec(dllexport)   
#else  
#define COLLADA_API __declspec(dllimport)   
#endif

namespace collada
{
	class Collada
	{
		std::shared_ptr<xmlParser::Node> root;
		std::map<std::string, const xmlParser::Node &> idToNode;
		std::shared_ptr<graphic::Scene> scene;

		void mapNodeById(const xmlParser::Node &);
		void createVisualScene(const xmlParser::Node &visualScene);
		void modifyInstance(const xmlParser::Node &node, ObjectInstance &instance);
	public:
		COLLADA_API void loadFile(const std::string &file);
		COLLADA_API std::shared_ptr<graphic::Scene> createScene();

		COLLADA_API const std::map<std::string, const xmlParser::Node &> &getIdToNode() const;
	};
}