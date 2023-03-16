#pragma once

#include "components/simple_scene.h"

class Game : public gfxc::SimpleScene
{
public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void RenderAndUpdateGameComponents(float);
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;

	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

	void RenderGameComponents();
	void RenderMinimap();
	void RenderScreenObjects();

	void UpdateGameComponents(float);
	void UpdateCar(float);

	void CreateShaders();

private:
	size_t m_nrOfStreetLights;
	size_t m_nrOfNPCs;
	size_t m_nrOfTrees;

	float m_curveCoefficent;

	glm::ivec2 m_resolution;

	bool m_carReset;
	bool m_frameTimer;
};