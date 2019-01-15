#include "stdafx.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb/stb_image.h>

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
	glEnableVertexAttribArray(index);
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
		OutputDebugStringA("[!] Compile vShader error\n");
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
		OutputDebugStringA("[!] Compile fShader error\n");
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

void graphic::Mesh::initTexture(const std::string &file)
{
	int width, height, nrChannels;
	unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	
		stbi_image_free(data);
	}
	else
	{
		OutputDebugStringA("Failed to load texture");
	}
}