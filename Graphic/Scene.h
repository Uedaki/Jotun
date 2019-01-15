#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Window.h"

#ifdef GRAPHIC_EXPORTS  
#define GRAPHIC_API __declspec(dllexport)   
#else  
#define GRAPHIC_API __declspec(dllimport)   
#endif

namespace graphic
{
	class Scene
	{
	public:
		enum DrawingMode
		{
			POINTS,
			LINES,
			TRIANGLES,

			END_OF_ENUM
		};

	private:
		Camera camera;
		GLenum drawingMode;
		std::vector<Mesh> meshes;

	public:
		GRAPHIC_API Scene();

		GRAPHIC_API Mesh &newMesh();

		GRAPHIC_API void draw();

		GRAPHIC_API void setDrawingMode(DrawingMode mode);
		GRAPHIC_API Camera &getCamera();
	};
}