#include "stdafx.h"
#include "ShaderGen.h"

graphic::ShaderGen::ShaderGen()
	: hasAmbient(false), needFragPos(false), needViewPos(false),
	objectSpecular("0")
{}

void graphic::ShaderGen::setObjectColor(const std::string &color)
{
	objectColor = color;
}

void graphic::ShaderGen::setObjectSpecular(const std::string &specular)
{
	objectSpecular = specular;
}

void graphic::ShaderGen::setShininess(float value)
{
	shininess = value;
}

void graphic::ShaderGen::addAmbientLight(AmbientLight &light)
{
	if (hasAmbient)
	{
		ambient.color = (ambient.color * ambient.intensity + light.color * light.intensity);
		ambient.color[0] = ambient.color[0] / 2;
		ambient.color[1] = ambient.color[1] / 2;
		ambient.color[2] = ambient.color[2] / 2;
		if (light.intensity > ambient.intensity)
			ambient.intensity = light.intensity;
	}
	else
	{
		ambient = light;
		hasAmbient = true;
	}
}

void graphic::ShaderGen::addPointLight(PointLight &light)
{
	pointLight.push_back(light);
	needFragPos = true;
	needViewPos = true;
}

void graphic::ShaderGen::setPointLightStruct(std::string &fShader)
{
	fShader += "struct PointLight\n"
		"{\n"
		"  vec3 position;\n"
		"  float intensity;\n\n"
		"  float constant;\n"
		"  float linear;\n"
		"  float quadratic;\n\n"
		"  vec3 ambient;\n"
		"  vec3 diffuse;\n"
		"  vec3 specular;\n"
		"};\n\n";

	fShader += "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
	"{\n"
	"	vec3 lightDir = normalize(light.position - fragPos);\n"
	"	// diffuse shading\n"
	"	float diff = max(dot(normal, lightDir), 0.0);\n"
	"	// specular shading\n"
	"	vec3 reflectDir = reflect(lightDir, normal);\n"
	"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), " + std::to_string(shininess) + ");\n"
	"	// attenuation\n"
	"	float distance = length(light.position - fragPos);\n"
	"	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
	"	// combine results\n"
	"	vec3 ambient = light.ambient * vec3(" + objectColor + ");\n"
	"	vec3 diffuse = light.diffuse * diff * vec3(" + objectColor + ");\n"
	"	vec3 specular = light.specular * spec * vec3(" + objectSpecular + ");\n"
	"	ambient *= attenuation;\n"
	"	diffuse *= attenuation * light.intensity;\n"
	"	specular *= attenuation * light.intensity;\n"
	"	return (ambient + diffuse + specular);\n"
	"}\n";
}

void graphic::ShaderGen::initPointLightStruct(std::string &fShader, PointLight &light)
{
	fShader += "  PointLight " + light.name + " = {\n"
		"          {" + std::to_string(light.position[0]) + ", " + std::to_string(light.position[1])
		+ ", " + std::to_string(light.position[2]) + "}, // position\n"
		"          " + std::to_string(light.intensity) + ", // intensity\n"
		"          " + std::to_string(light.constant) + ", // constant\n"
		"          " + std::to_string(light.linear) + ", // linear\n"
		"          " + std::to_string(light.quadratic) + ", // quadratic\n"
		"          {" + std::to_string(light.ambient[0]) + ", " + std::to_string(light.ambient[1])
		+ ", " + std::to_string(light.ambient[2]) + "}, // ambient color\n"
		"          {" + std::to_string(light.diffuse[0]) + ", " + std::to_string(light.diffuse[1])
		+ ", " + std::to_string(light.diffuse[2]) + "}, // diffuse color\n"
		"          {" + std::to_string(light.specular[0]) + ", " + std::to_string(light.specular[1])
		+ ", " + std::to_string(light.specular[2]) + "}, // specular color\n"
		"  };\n\n";
}

void graphic::ShaderGen::addSpotLight(SpotLight &light)
{
	spotLight.push_back(light);
	needFragPos = true;
	needViewPos = true;
}

void graphic::ShaderGen::setSpotLightStruct(std::string &fShader)
{
	fShader += "struct SpotLight\n"
		"{\n"
		"  vec3 position;\n"
		"  vec3 direction;\n"
		"  float angle;\n"
		"  float intensity;\n\n"
		"  float constant;\n"
		"  float linear;\n"
		"  float quadratic;\n\n"
		"  vec3 ambient;\n"
		"  vec3 diffuse;\n"
		"  vec3 specular;\n"
		"};\n\n";

	fShader += "vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
		"{\n"
		"	vec3 lightDir = normalize(light.position - fragPos);\n"
		"	// diffuse shading\n"
		"	float diff = max(dot(normal, lightDir), 0.0);\n"
		"	// specular shading\n"
		"	vec3 reflectDir = reflect(-lightDir, normal);\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), " + std::to_string(shininess) + ");\n"
		"	// attenuation\n"
		"	float distance = length(light.position - fragPos);\n"
		"	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
		"	// spotlight intensity\n"
		"	float theta = dot(lightDir, normalize(-light.direction));\n"
		"   float epsilon = 0.01;\n"
		"	float intensity = clamp((theta - light.angle - epsilon) / epsilon, 0.0, 1.0);\n"
		"	// combine results\n"
		"	vec3 ambient = light.ambient * vec3(" + objectColor + ");\n"
		"	vec3 diffuse = light.diffuse * diff * vec3(" + objectColor + ");\n"
		"	vec3 specular = light.specular * spec * vec3(" + objectSpecular + ");\n"
		"	ambient *= attenuation * intensity * light.intensity;\n"
		"	diffuse *= attenuation * intensity * light.intensity;\n"
		"	specular *= attenuation * intensity * light.intensity;\n"
		"	return (ambient + diffuse + specular);\n"
		"}\n";
}

void graphic::ShaderGen::initSpotLightStruct(std::string &fShader, SpotLight &light)
{
	fShader += "  SpotLight " + light.name + " = {\n"
		"          {" + std::to_string(light.position[0]) + ", " + std::to_string(light.position[1])
		+ ", " + std::to_string(light.position[2]) + "}, // position\n"
		"          {" + std::to_string(light.direction[0]) + ", " + std::to_string(light.direction[1])
		+ ", " + std::to_string(light.direction[2]) + "}, // direction\n"
		"          " + std::to_string(light.angle) + ", // angle\n"
		"          " + std::to_string(light.intensity) + ", // intensity\n"
		"          " + std::to_string(light.constant) + ", // constant\n"
		"          " + std::to_string(light.linear) + ", // linear\n"
		"          " + std::to_string(light.quadratic) + ", // quadratic\n"
		"          {" + std::to_string(light.ambient[0]) + ", " + std::to_string(light.ambient[1])
		+ ", " + std::to_string(light.ambient[2]) + "}, // ambient color\n"
		"          {" + std::to_string(light.diffuse[0]) + ", " + std::to_string(light.diffuse[1])
		+ ", " + std::to_string(light.diffuse[2]) + "}, // diffuse color\n"
		"          {" + std::to_string(light.specular[0]) + ", " + std::to_string(light.specular[1])
		+ ", " + std::to_string(light.specular[2]) + "}, // specular color\n"
		"  };\n\n";
}

void graphic::ShaderGen::generateVertexShader(std::string &vShader)
{
	vShader.clear();
	vShader += "#version 330 core\n";

	for (uint8_t idx = 0; idx < inputs.size(); ++idx)
	{
		vShader += "layout (location = " + std::to_string(idx) + ") in " + inputs[idx].type + " a" + inputs[idx].variable + ";\n";
	}

	vShader += "\nuniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n\n";

	if (needFragPos)
		vShader += "out vec3 FragPos;\n";
	for (auto &input : inputs)
	{
		if (input.hasToBeTransfer)
			vShader += "out " + input.type + " " + input.variable + ";\n";
	}

	vShader += "\n"
		"void main()\n"
		"{\n";

	if (needFragPos)
		vShader += "  FragPos = vec3(model * vec4(aPOSITION, 1.0));\n";
	for (auto &input : inputs)
	{
		if (input.hasToBeTransfer && input.variable == "NORMAL")
			vShader += "  " + input.variable + " = mat3(transpose(inverse(model))) * a" + input.variable + ";\n";
		else if (input.hasToBeTransfer)
			vShader += "  " + input.variable + " = a" + input.variable + ";\n";
	}

	vShader += "\n  gl_Position = projection * view * model * vec4(aPOSITION, 1.0f);\n}\n";
}

void graphic::ShaderGen::generateFragShader(std::string &fShader)
{
	fShader.clear();
	fShader += "#version 330 core\n"
		"out vec4 FragColor;\n";

	if (needViewPos)
		fShader += "\nuniform vec3 viewPos;\n";

	fShader += "vec3 addition(vec3 a, vec3 b)"
		"{\n"
		"  if (a[0] < b[0])\n"
		"  {\n"
		"    a[0] = b[0];\n"
		"  }\n"
		"  if (a[1] < b[1])"
		"  {\n"
		"    a[1] = b[1];\n"
		"  }\n"
		"  if (a[2] < b[2])\n"
		"  {\n"
		"    a[2] = b[2];\n"
		"  }\n"
		"  return (a);\n"
		"}\n";

	fShader += "\n";
	if (needFragPos)
		fShader += "in vec3 FragPos;\n";
	for (auto &input : inputs)
	{
		if (input.hasToBeTransfer)
			fShader += "in " + input.type + " " + input.variable + ";\n";
	}
	fShader += "\n";

	for (auto &uni : fragUniform)
	{
		fShader += "uniform " + uni.second + " " + uni.first + ";\n";
	}

	if (pointLight.size() != 0)
	{
		setPointLightStruct(fShader);
	}
	if (spotLight.size() != 0)
	{
		setSpotLightStruct(fShader);
	}

	fShader += "void main()\n"
		"{\n";

	for (auto &input : inputs)
	{
		if (input.variable == "NORMAL" && input.hasToBeTransfer)
		{
			fShader += "  vec3 normal = normalize(NORMAL);\n";
			break;
		}
	}
	if (needViewPos)
		fShader += "  vec3 viewDir = normalize(viewPos - FragPos);\n\n";
	
	for (auto &light : pointLight)
	{
		initPointLightStruct(fShader, light);
	}

	for (auto &light : spotLight)
	{
		initSpotLightStruct(fShader, light);
	}

	if (!hasAmbient && pointLight.size() == 0 && spotLight.size() == 0)
		fShader += "  vec3 result = vec3(1, 1, 1);\n";
	else if (!hasAmbient)
		fShader += "  vec3 result = vec3(0, 0, 0);\n";
	else
	{
		ambient.color = ambient.intensity * ambient.color;
		fShader += "  vec3 result = vec3(" + std::to_string(ambient.color[0]) + ", " + std::to_string(ambient.color[1])
			+ ", " + std::to_string(ambient.color[2]) + ");\n";
	}
	
	for (auto &light : pointLight)
	{
		fShader += "  result = addition(result, CalcPointLight(" + light.name + ", normal, FragPos, viewDir));\n";
	}

	for (auto &light : spotLight)
	{
		fShader += "  result = addition(result, CalcSpotLight(" + light.name + ", normal, FragPos, viewDir));\n";
	}
	
	fShader += "\n  FragColor = ";
	if (objectColor.empty())
		fShader += "vec4(1.0, 0.4, 0.4, 1.0)";
	else
		fShader += objectColor;
	fShader += " * vec4(result, 1);\n}\n";
}

void graphic::ShaderGen::setInput(const std::string &variable, const std::string &type)
{
	inputs.emplace_back();
	inputs.back().variable = variable;
	inputs.back().type = type;
	inputs.back().hasToBeTransfer = false;
}

void graphic::ShaderGen::require(const std::string &variable, graphic::ShaderGen::ShaderType type)
{
	if (type == ShaderType::VERTEX)
		return;
	for (auto &input : inputs)
	{
		if (input.variable == variable)
			input.hasToBeTransfer = true;
	}
}

void graphic::ShaderGen::addFragUniform(const std::string &name, const std::string &type)
{
	fragUniform.emplace_back(name, type);
}
