#include "stdafx.h"
#include <glad/glad.h>
#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>

static int width;
static int height;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	width = width;
	height = height;
	glViewport(0, 0, width, height);
}

graphic::Window::Context::Context()
	: width(400), height(400), bckgdColor(0.1529, 0.1568, 0.1490)
{}

graphic::Window::Window(const std::string &title, const graphic::Window::Context &context)
	: bckgdColor(context.bckgdColor)
{
	width = context.width;
	height = context.height;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!this->win)
	{
		glfwTerminate();
		throw std::runtime_error("[OpenGlGraphic] Failed to create window");
	}
	glfwMakeContextCurrent(this->win);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("[OpenGlGraphic] Failed to load OpenGl address");
	}

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	glClearColor(bckgdColor[0], bckgdColor[1], bckgdColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
}

graphic::Window::~Window()
{
	glfwSetWindowShouldClose(this->win, true);
	glfwTerminate();
}

void graphic::Window::render()
{
	glfwSwapBuffers(win);
	glfwPollEvents();
	glClearColor(bckgdColor[0], bckgdColor[1], bckgdColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int graphic::Window::isKeyPressed(int key)
{
	return (glfwGetKey(this->win, key));
}

bool graphic::Window::isRunning()
{
	return (!glfwWindowShouldClose(win));
}

void graphic::Window::resize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	glfwSetWindowSize(win, width, height);
}

int graphic::Window::getWidth() const
{
	return (width);
}

int graphic::Window::getHeight() const
{
	return (height);
}