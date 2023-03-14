#pragma once

#include "Utilities/Camera.hpp"
#include "components/simple_scene.h"

#include "Game/GameComponents/Tree.hpp"
#include "Game/GameComponents/Track.hpp"
#include "Game/GameComponents/Car.hpp"
#include "Game/GameComponents/Field.hpp"

#include "Game/Physics/Collision/CollisionEngine.hpp"
#include "Game/Utilities/Utilities.hpp"

#include "Game/Display/ScreenElements.hpp"
#include "Game/Display/Minimap.hpp"

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
	std::vector<std::unique_ptr<GameComponent>> m_components;

	std::shared_ptr<CustomCamera> m_camera;
	std::unique_ptr<Car> m_car;

	DTimer frametimer;
	DTimer m_resetTimer;

	size_t m_nrOfStreetLights;
	size_t m_nrOfNPCs;
	size_t m_nrOfTrees;

	float m_curveCoefficent;

	glm::ivec2 m_resolution;

	std::unique_ptr<ScreenElements> m_screen;
	std::shared_ptr<MiniMap> m_minimap;

	bool m_carReset;
	bool m_frameTimer;
};