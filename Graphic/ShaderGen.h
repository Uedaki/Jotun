#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#ifdef GRAPHIC_EXPORTS  
#define GRAPHIC_API __declspec(dllexport)   
#else  
#define GRAPHIC_API __declspec(dllimport)   
#endif

namespace graphic
{
	class ShaderGen
	{
	public:
		enum ShaderType
		{
			VERTEX,
			FRAGMENT
		};

		struct AmbientLight
		{
			glm::vec3 color;
			float intensity;
		};

		struct PointLight
		{
			std::string name;
			glm::vec3 position;
			float intensity;

			float constant;
			float linear;
			float quadratic;

			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};

		struct SpotLight {
			std::string name;
			glm::vec3 position;
			glm::vec3 direction;
			float angle;
			float intensity;

			float constant;
			float linear;
			float quadratic;

			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};

	protected:
		struct Input
		{
			std::string variable;
			std::string type;
			bool hasToBeTransfer;
		};


		bool hasAmbient;
		bool needFragPos;
		bool needViewPos;

		float shininess;
		std::string objectColor;
		std::string objectSpecular;

		AmbientLight ambient;
		std::vector<PointLight> pointLight;
		std::vector<SpotLight> spotLight;

		std::vector<Input> inputs;
		std::vector<std::pair<std::string, std::string>> fragUniform; // name, type
	
	public:
		GRAPHIC_API ShaderGen();

		GRAPHIC_API void setInput(const std::string &variable, const std::string &type);
		GRAPHIC_API void require(const std::string &variable, ShaderType type);
		GRAPHIC_API void addFragUniform(const std::string &name, const std::string &type);

		GRAPHIC_API void setObjectColor(const std::string &color);
		GRAPHIC_API void setObjectSpecular(const std::string &specular);
		GRAPHIC_API void setShininess(float value);
		GRAPHIC_API void addAmbientLight(AmbientLight &newAmbientLight);

		GRAPHIC_API void addPointLight(PointLight &newPointLight);
		GRAPHIC_API void setPointLightStruct(std::string &fShader);
		GRAPHIC_API void initPointLightStruct(std::string &fShader, PointLight &light);

		GRAPHIC_API void addSpotLight(SpotLight &newPointLight);
		GRAPHIC_API void setSpotLightStruct(std::string &fShader);
		GRAPHIC_API void initSpotLightStruct(std::string &fShader, SpotLight &light);

		GRAPHIC_API void generateVertexShader(std::string &vShader);
		GRAPHIC_API void generateFragShader(std::string &vShader);
	};
}