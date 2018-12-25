#include "stdafx.h"
#include "Mesh.h"

#include <sstream>
#include <cstdint>

collada::Mesh::Mesh(const xmlParser::Node &node)
	: isLoaded(false)
{
	OutputDebugStringA("- Importing mesh\n");

	retreiveSources(node);
	for (auto &source : sources)
	{
		accessSource.emplace(source.name, source);
	}
	
	const xmlParser::Node &vertices = node["vertices"];
	const std::string &verticesUrl = vertices["input"].getParameters().at("source").getString();
	accessSource.emplace(vertices.getParameters().at("id").getString(), accessSource.at(verticesUrl.substr(1, verticesUrl.length() - 1)));

	createTriangles(node);
}

void collada::Mesh::retreiveSources(const xmlParser::Node &node)
{
	for (auto &sourceNode : node.getChildren())
	{
		if (sourceNode.getTag() == "source")
		{
			sources.push_back(Source());
			Source &source = sources.back();
			source.name = sourceNode.getParameters().at("id").getString();

			source.stride = sourceNode["technique_common"]["accessor"].getParameters().at("stride").getNumber<uint16_t>();
			
			std::string elem;
			std::istringstream stream(sourceNode["float_array"].getContent().getString());
			while (stream.peek() != -1)
			{
				stream >> elem;
				source.data.push_back(std::stof(elem));
			}
		}
	}
}

void collada::Mesh::createTriangles(const xmlParser::Node &node)
{
	for (auto &child : node.getChildren())
	{
		if (child.getTag() == "triangles")
		{
			triangles.emplace_back();
			std::vector<Source*> inputs;

			std::vector<uint32_t> indices;
			Triangles &triangle = triangles.back();

			triangle.nbrTriangles = child.getParameters().at("count").getNumber<uint32_t>();

			for (auto &input : child.getChildren())
			{
				if (input.getTag() == "input")
				{
					uint16_t offset = input.getParameters().at("offset").getNumber<uint16_t>();

					if (triangle.semantic.size() <= offset)
					{
						for (size_t idx = triangle.semantic.size(); idx < offset + 1; ++idx)
						{
							triangle.semantic.emplace_back();
							inputs.push_back(nullptr);
						}
					}
					
					const std::string inputUrl = input.getParameters().at("source").getString();
					inputs[offset] = &accessSource.at(inputUrl.substr(1, inputUrl.length() - 1));

					triangle.semantic[offset].name = input.getParameters().at("semantic").getString();
					triangle.semantic[offset].size = inputs[offset]->stride;
				}
			}

			uint32_t pos = 0;
			std::string elem;
			std::istringstream stream(child["p"].getContent().getString());
			while (stream.peek() != -1)
			{
				stream >> elem;
				uint32_t indice = std::stoul(elem);
				for (uint32_t idx = indice * inputs[pos % inputs.size()]->stride; idx < indice * inputs[pos % inputs.size()]->stride + inputs[pos % inputs.size()]->stride; ++idx)
				{
					triangle.data.push_back(inputs[pos % inputs.size()]->data[idx]);
				}
				pos += 1;
			}

			triangle.textureId = child.getParameters().at("material").getString();
		}
	}
}

const char *vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoord;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
//"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\n\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void collada::Mesh::instanciate(graphic::Scene &scene, glm::mat4 transform)
{
	for (auto &triangle : triangles)
	{
		graphic::Mesh &mesh = scene.newMesh();

		mesh.model = transform;
		mesh.nbrVertex = triangle.nbrTriangles * 3;

		mesh.vertex = triangle.data;
		mesh.initVertexBuffer();

		uint32_t idx = 0;
		uint32_t offset = 0;
		uint32_t fullSize = 0;
		for (auto &item : triangle.semantic)
			fullSize += item.size;
		for (auto &item : triangle.semantic)
		{
			mesh.mapDataFromVertexBuffer(idx, item.size, offset, fullSize);
			offset += item.size;
			idx += 1;
		}

		mesh.initShader(vertexShaderSource, fragmentShaderSource);
	}
}