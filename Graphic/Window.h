#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#ifdef GRAPHIC_EXPORTS  
#define GRAPHIC_API __declspec(dllexport)   
#else  
#define GRAPHIC_API __declspec(dllimport)   
#endif

namespace graphic
{
	class Window
	{
		struct Context
		{
			int width;
			int height;
			glm::vec3 bckgdColor;

			GRAPHIC_API Context();
		};

	private:
		glm::vec3 bckgdColor;
		GLFWwindow *win;

	public:

		GRAPHIC_API Window(const std::string &title = "Untitled", const Context &context = Context());
		GRAPHIC_API ~Window();

		GRAPHIC_API void render();

		GRAPHIC_API glm::vec2 getMousePos();
		GRAPHIC_API int checkKeyboard(int key);
		GRAPHIC_API int checkMouse(int key);
		GRAPHIC_API bool isRunning();

		GRAPHIC_API void resize(int width, int height);
		GRAPHIC_API int getWidth() const;
		GRAPHIC_API int getHeight() const;

	};
}