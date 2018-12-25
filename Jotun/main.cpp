#include <Graphic/Window.h>
#include <Graphic/Scene.h>
#include <chrono>
#include <Collada/Collada.h>

#define getTime std::chrono::high_resolution_clock::now
#define calcDelta(a, b) std::chrono::duration<float>(a - b).count()

int main()
{
	graphic::Window win;

	collada::Collada col;
	col.loadFile("ColladaFile/simpleCamera.dae");
	std::shared_ptr<graphic::Scene> scene = col.createScene();

	auto lastTick = getTime();
	while (win.isRunning())
	{
		auto currTick = getTime();
		float delta = calcDelta(currTick, lastTick);
		
		scene->getCamera().movement(win, delta);
		scene->draw();
		win.render();

		lastTick = currTick;
	}

	return (0);
}