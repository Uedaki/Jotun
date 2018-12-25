#pragma once

#include <string>
#include <vector>
#include <XmlParser/Node.h>
#include "Geometry.h"

#ifdef COLLADA_EXPORTS  
#define COLLADA_API __declspec(dllexport)   
#else  
#define COLLADA_API __declspec(dllimport)   
#endif

namespace collada
{
	class Mesh : public Geometry
	{
		struct Source
		{
			std::string name;
			uint16_t stride;
			std::vector<float> data;
		};

		struct Semantic
		{
			std::string name;
			uint32_t size;
		};

		struct Triangles
		{
			uint32_t nbrTriangles;
			std::vector<float> data;
			std::vector<Semantic> semantic;
			std::string textureId;
			std::string vShader;
			std::string fShader;
		};

		std::string name;
		bool isLoaded;

		std::vector<Source> sources;
		std::map<std::string, Source&> accessSource;

		std::vector<Triangles> triangles;

		void retreiveSources(const xmlParser::Node &node);
		void createTriangles(const xmlParser::Node &node);
	public:
		COLLADA_API Mesh(const xmlParser::Node &node);

		COLLADA_API void instanciate(graphic::Scene &scene, glm::mat4 transform) override;
	};
}