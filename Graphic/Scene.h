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
		Camera camera;
		GLenum drawingMode;
		std::vector<Mesh> meshes;

	public:
		Scene()
			: drawingMode(GL_TRIANGLES)
		{}

		GRAPHIC_API Mesh &newMesh();

		GRAPHIC_API void draw();
		GRAPHIC_API void handleCamera(Window &context, float delta);

		GRAPHIC_API void setDrawingMode(GLenum mode);
		GRAPHIC_API Camera &getCamera();
	};
}