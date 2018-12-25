#include "stdafx.h"
#include <glad/glad.h>
#include "Scene.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

graphic::Mesh &graphic::Scene::newMesh()
{
	meshes.emplace_back();
	return (meshes.back());
}

void graphic::Scene::draw()
{
	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 proj = camera.getProjectionMatrix();

	for (const Mesh &mesh : meshes)
	{
		unsigned int modelLoc = glGetUniformLocation(mesh.shader, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.model));

		unsigned int viewLoc = glGetUniformLocation(mesh.shader, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projLoc = glGetUniformLocation(mesh.shader, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glUseProgram(mesh.shader);
		glBindVertexArray(mesh.VAO);
		glDrawArrays(drawingMode, 0, mesh.nbrVertex);
	}
}

void graphic::Scene::setDrawingMode(GLenum mode)
{
	drawingMode = mode;
}

graphic::Camera &graphic::Scene::getCamera()
{
	return (camera);
}