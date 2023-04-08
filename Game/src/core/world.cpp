#include "core/world.h"

#include "components/camera_input.h"
#include "components/transform.h"
#include "core/engine.h"


Scene::Scene()
{
	previousTime = 0;
	elapsedTime = 0;
	deltaTime = 0;
	paused = false;
	shouldClose = false;

	window = Engine::GetWindow();
}


void Scene::Run()
{
	if (!window)
		return;

	while (!window->ShouldClose() && !shouldClose)
	{
		LoopUpdate();
	}
}


void Scene::Pause()
{
	paused = !paused;
}


void Scene::Exit()
{
	shouldClose = true;
	window->Close();
}

void Scene::CloseScene()
{
	shouldClose = true;
}


double Scene::GetLastFrameTime()
{
	return deltaTime;
}


void Scene::ComputeFrameDeltaTime()
{
	// elapsedTime = Engine::GetElapsedTime();
	// deltaTime = elapsedTime - previousTime;
	// previousTime = elapsedTime;

	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime =
		std::chrono::high_resolution_clock::now();

	std::chrono::nanoseconds elapsedTime =
		std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastFrameTime);
	
	deltaTime = elapsedTime.count() / 1e+9;

	//lastFrameTime = currentTime;

	if (elapsedTime < std::chrono::nanoseconds{ 166'666'666 })
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds{166'666'666} - elapsedTime);
	}

	lastFrameTime = std::chrono::high_resolution_clock::now();
}


void Scene::LoopUpdate()
{
	// Polls and buffers the events
	window->PollEvents();

	// Computes frame deltaTime in seconds
	ComputeFrameDeltaTime();

	// Calls the methods of the instance of InputController in the following order
	// OnWindowResize, OnMouseMove, OnMouseBtnPress, OnMouseBtnRelease, OnMouseScroll, OnKeyPress,
	// OnMouseScroll, OnInputUpdate OnInputUpdate will be called each frame, the other functions are
	// called only if an event is registered
	window->UpdateObservers();

	// Frame processing
	FrameStart();
	Update(static_cast<float>(deltaTime));
	Render(static_cast<float>(deltaTime));
	FrameEnd();

	// Swap front and back buffers - image will be displayed to the screen
	window->SwapBuffers();
}
