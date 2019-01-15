#include "stdafx.h"
#include "Mesh.h"

#include <sstream>
#include <cstdint>
#include <Graphic/ShaderGen.h>

collada::Mesh::Mesh(const xmlParser::Node &node)
	: isLoaded(false)
{
	OutputDebugStringA("- Importing mesh\n");

	retreiveSources(node);
	for (auto &source : sources)
	{
		accessSource.emplace(source.name, source);
	}

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
			Triangles &triangle = triangles.back();
			triangle.nbrTriangles = child.getParameters().at("count").getNumber<uint32_t>();
			triangle.material = child.getParameters().at("material").getString();

			std::vector<Source*> inputs;
			int offsetLimit = fillInputs(triangle, inputs, node["vertices"], child);

			std::vector<uint32_t> indices;
			std::istringstream stream(child["p"].getContent().getString());
			while (stream.peek() != -1)
			{
				std::string elem;
				stream >> elem;
				indices.push_back(std::stoul(elem));
			}

			fillVertexData(triangle, inputs, indices, offsetLimit);
		}
	}
}

int collada::Mesh::fillInputs(collada::Mesh::Triangles &triangle, std::vector<Source *> &inputs, const xmlParser::Node &vertices, const xmlParser::Node &node)
{
	int maxOffset = 0;

	for (auto &input : node.getChildren())
	{
		if (input.getTag() == "input")
		{
			uint16_t offset = input.getParameters().at("offset").getNumber<uint16_t>();
			if (offset > maxOffset)
				maxOffset = offset;

			const std::string &inputUrl = input.getParameters().at("source").getString();
			std::string url = inputUrl.substr(1, inputUrl.length() - 1);
			if (url != vertices.getParameter("id").getString())
			{
				triangle.semantic.emplace_back();
				inputs.emplace_back(&accessSource.at(url));
				inputs.back()->offset = offset;
				triangle.semantic.back().name = input.getParameters().at("semantic").getString();
				triangle.semantic.back().size = inputs.back()->stride;
			}
			else
			{
				for (auto &child : vertices.getChildren())
				{
					if (child.getTag() == "input")
					{
						const std::string &vertUrl = child.getParameters().at("source").getString();
						triangle.semantic.emplace_back();
						inputs.emplace_back(&accessSource.at(vertUrl.substr(1, vertUrl.length() - 1)));
						inputs.back()->offset = offset;
						triangle.semantic.back().name = child.getParameters().at("semantic").getString();
						triangle.semantic.back().size = inputs.back()->stride;
					}
				}
			}
		}
	}
	return (maxOffset + 1);
}

void collada::Mesh::fillVertexData(collada::Mesh::Triangles &triangle, std::vector<Source *> &inputs, std::vector<uint32_t> indices, int offsetLimit)
{
	for (uint32_t indiceIdx = 0; indiceIdx < indices.size(); indiceIdx += offsetLimit)
	{
		for (auto input : inputs)
		{
			for (uint32_t dataIdx = indices[indiceIdx + input->offset] * input->stride; dataIdx < indices[indiceIdx + input->offset] * input->stride + input->stride; ++dataIdx)
			{
				triangle.data.push_back(input->data[dataIdx]);
			}
		}
	}
}

#include <iostream>

void collada::Mesh::setMaterial(const std::map<std::string, std::shared_ptr<Material>> &materials)
{
	for (auto &triangle : triangles)
	{
		if (materials.find(triangle.material) != materials.cend())
		{
			graphic::ShaderGen gen;

			for (auto &input : triangle.semantic)
			{
				std::string type = "vec";
				type += std::to_string(input.size);
				gen.setInput(input.name, type);
			}

			materials.at(triangle.material)->setShader(gen);
			materials.at(triangle.material)->getTextures(triangle.texture);

			gen.generateVertexShader(triangle.vShader);
			gen.generateFragShader(triangle.fShader);

			std::cout << "--------- Vertex shader\n"
				<< triangle.vShader
				<< "\n\n --------- Fragment shader\n"
				<< triangle.fShader << std::endl;
		}
	}
}

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

		mesh.initShader(triangle.vShader, triangle.fShader);

		if (!triangle.texture.empty())
			mesh.initTexture(triangle.texture);
	}
}