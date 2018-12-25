#include "stdafx.h"
#include "Mesh.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

graphic::Mesh::Mesh()
	: model(glm::mat4(1))
{}

void graphic::Mesh::initVertexBuffer(unsigned int usage)
{
	if (this->vertex.empty())
	{
		OutputDebugStringA("[!] Could not initialize empty vertex buffer");
		return;
	}

	glGenBuffers(1, &VBO);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), usage);
}

void graphic::Mesh::mapDataFromVertexBuffer(uint32_t index, uint32_t elementSize, uint32_t offset, uint32_t sectionSize)
{
	glVertexAttribPointer(index, elementSize, GL_FLOAT, GL_FALSE, sectionSize * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
	glEnableVertexAttribArray(0);
}

void graphic::Mesh::initShader(const std::string &vCode, const std::string &fCode)
{
	unsigned int vShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);

	const char *vCodeChar = vCode.c_str();
	glShaderSource(vShader, 1, &vCodeChar, nullptr);
	glCompileShader(vShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
		OutputDebugStringA("[!] Compile vShader error: ");
		OutputDebugStringA(infoLog);
	}

	unsigned int fShader;
	const char *fCodeChar = fCode.c_str();
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fCodeChar, nullptr);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
		OutputDebugStringA("[!] Compile vShader error: ");
		OutputDebugStringA(infoLog);
	}

	shader = glCreateProgram();

	glAttachShader(shader, vShader);
	glAttachShader(shader, fShader);
	glLinkProgram(shader);

	glGetProgramiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(vShader, 512, nullptr, infoLog);
		OutputDebugStringA("[!] Compile Shader error: ");
		OutputDebugStringA(infoLog);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}