#include "stdafx.h"
#include <glad/glad.h>
#include "Scene.h"

#include <map>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

graphic::Scene::Scene()
{
	setDrawingMode(TRIANGLES);
}

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

		if (mesh.texture != 0)
			glBindTexture(GL_TEXTURE_2D, mesh.texture);

		glUseProgram(mesh.shader);
		glBindVertexArray(mesh.VAO);
		glDrawArrays(drawingMode, 0, mesh.nbrVertex);
	}
}

void graphic::Scene::setDrawingMode(graphic::Scene::DrawingMode mode)
{
	std::vector<GLenum> glMode(DrawingMode::END_OF_ENUM);
	glMode[POINTS] = GL_POINTS;
	glMode[LINES] = GL_LINES;
	glMode[TRIANGLES] = GL_TRIANGLES;

	if (mode >= 0 && mode < END_OF_ENUM)
		drawingMode = glMode[mode];
}

graphic::Camera &graphic::Scene::getCamera()
{
	return (camera);
}