#pragma once

#include "components/simple_scene.h"

#include "amGame/GameComponents/GameComponent.hpp"
#include "amGame/GameComponents/TrackBuilder.hpp"
#include "amGame/GameComponents/Car.hpp"
#include "amGame/GameComponents/Field.hpp"
#include "amGame/GameComponents/PlaceTracker.hpp"
#include "amGame/GameComponents/StreetLight.hpp"
#include "amGame/GameComponents/Tree.hpp"
#include "amGame/GameComponents/NPCCar.hpp"

#include "amGame/Display/ScreenElements.hpp"

#include "amGame/Lighting/LightSourceVector.hpp"

class Game : public gfxc::SimpleScene
{
public:
	Game();
	~Game();

	void Init() override;

private:
	void FrameStart() override;
	void Render(float deltaTime) override;
	void Update(float deltaTime) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;

	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

	void CreateShaders();
	void CreateTextures();

private:
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures;
	std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;

	std::unique_ptr<CustomCamera> m_camera;
	std::shared_ptr<CustomCamera> m_cameraAttachedToCar;

	LightSourcesVector m_lightSources;

	std::unique_ptr<ScreenElements> m_screenElements;

	bool m_lightingEnabled = true;
	std::string shaderToUse;

	// Game Components
	std::unique_ptr<Track> m_track;
	std::unique_ptr<Car> m_car;
	std::unique_ptr<Field> m_field;
	std::vector<std::unique_ptr<StreetLight>> m_streetLights;
	std::vector<std::unique_ptr<Tree>> m_trees;
	std::vector<std::unique_ptr<NPCCar>> m_npcs;

	ForcedFramerateTimer framerateTimer;

	FTimer justInCase;
};