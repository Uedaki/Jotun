#pragma once

#include <vector>
#include <glm/glm.hpp>

#ifdef GRAPHIC_EXPORTS  
#define GRAPHIC_API __declspec(dllexport)   
#else  
#define GRAPHIC_API __declspec(dllimport)   
#endif

namespace graphic
{
	struct Mesh
	{
		glm::mat4 model;
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
		unsigned int shader;
		unsigned int nbrVertex;
		std::vector<float> vertex;
		
		GRAPHIC_API Mesh();
		GRAPHIC_API void initVertexBuffer(unsigned int usage = 0x88E4 /* GL_STATIC_DRAW */);
		GRAPHIC_API void mapDataFromVertexBuffer(uint32_t index, uint32_t elementSize, uint32_t offset, uint32_t sectionSize);
		GRAPHIC_API void initShader(const std::string &vShader, const std::string &fShader);
	};
}