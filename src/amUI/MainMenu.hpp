#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "Button.hpp"

class MainMenu : public Scene
{
public:
	using GameMode = GameSettings::GameMode;

	MainMenu();
	~MainMenu();

	void Init() override;

	void UpdateGameSettings();

private:
	void FrameStart() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;
	void FrameEnd() override;

	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;

	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

private:
	using RectangularButton = Button<Polygon2d>;
	using RectangularButtonPtr = std::unique_ptr<RectangularButton>;

	enum class MenuStates
	{
		INITIAL_MENU = 0,
		CHOSE_GAME_TYPE_MENU = 1,
		SINGLE_OR_MULTI_PLAYER_SPECIFIC_MENU = 2
	};

	void IncrementMenuState();
	void DecrementMenuState();

private:
	const utils::LogicSpace m_NDCSpace;
	const utils::LogicSpace m_logicSpace;

	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<TextEngine> m_textEngine;

	glm::mat3 m_logicToNDCSpaceMatrix;

	// Butoanele
	RectangularButtonPtr m_goBackButton;
	RectangularButtonPtr m_goToChoseMenuButton;
	RectangularButtonPtr m_singlePlayerChoseButton;
	RectangularButtonPtr m_multiplayerChoseButton;

	std::unique_ptr<GeometryObject2d> test;

	std::string teststring;

	MenuStates m_currentMenuState;
	GameMode m_gameMode;
};