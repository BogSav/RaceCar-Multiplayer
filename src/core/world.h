#pragma once

#include "window/input_controller.h"

#include <chrono>


class Scene : public InputController
{
public:
	Scene();
	virtual ~Scene() {}
	virtual void Init() {}
	virtual void FrameStart() {}
	virtual void Update(float deltaTime) {}
	virtual void Render(float deltaTime) {}
	virtual void FrameEnd() {}

	void Run();
	void Pause();
	void Exit();
	void CloseScene();

	double GetLastFrameTime();

private:
	void ComputeFrameDeltaTime();
	void LoopUpdate();

private:
	double previousTime;
	double elapsedTime;
	double deltaTime;
	bool paused;
	bool shouldClose;

	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime =
		std::chrono::high_resolution_clock::now();
};
