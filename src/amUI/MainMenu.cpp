#include "MainMenu.hpp"

#include <iostream>

MainMenu::MainMenu()
	: m_NDCSpace(-1.f, -1.f, 2.f, 2.f),
	  m_logicSpace(
		  0,
		  0,
		  static_cast<float>(window->GetResolution().x),
		  static_cast<float>(window->GetResolution().y))
{
	m_textEngine = std::make_unique<TextEngine>(window);
}

MainMenu::~MainMenu()
{
	InputController::SetActive(false);
}

void MainMenu::Init()
{
	m_logicToNDCSpaceMatrix = utils::VisualizationTransf2d(m_logicSpace, m_NDCSpace);

	// Creare shader 2d pentru menmiu
	{
		m_shader = std::make_unique<Shader>("TestShader");
		m_shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"2dShaders",
				"2dVertexShader.glsl"),
			GL_VERTEX_SHADER);
		m_shader->AddShader(
			PATH_JOIN(
				window->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"2dShaders",
				"2dFragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		m_shader->CreateAndLink();
	}

	// Creare butoane
	{
		std::shared_ptr<Polygon2d> defaultRectangle =
			std::make_shared<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red, true);

		m_goBackButton = std::make_unique<PolygonUIElement>(defaultRectangle, 150, 100, 50, 25);
		m_goToChoseMenuButton = std::make_unique<PolygonUIElement>(
			defaultRectangle,
			window->GetResolution().x / 2.f,
			window->GetResolution().y / 2.f,
			100.f,
			50.f);
		m_singlePlayerChoseButton = std::make_unique<PolygonUIElement>(
			defaultRectangle,
			window->GetResolution().x / 12.f * 4.f,
			window->GetResolution().y / 2.f,
			100.f,
			50.f);
		m_multiplayerChoseButton = std::make_unique<PolygonUIElement>(
			defaultRectangle,
			window->GetResolution().x / 12.f * 8.f,
			window->GetResolution().y / 2.f,
			100.f,
			50.f);
	}

	// Creare state menu si mod initial
	{
		m_currentMenuState = MenuStates::INITIAL_MENU;
		m_gameMode = GameMode::UNSELECTED;
	}

	// test = std::make_unique<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red);
}

void MainMenu::UpdateGameSettings(GameSettings* gameSettings)
{
	// Momentan le lasam valori default pana sa puitem alege din UI parametrii
	gameSettings->InitParameters();

	gameSettings->m_isMultiplayer = m_gameMode == GameMode::MULTIPLAYER;
	gameSettings->m_gameMode = m_gameMode;
	gameSettings->m_nrOfPlayers = static_cast<int>(m_gameMode);

	gameSettings->m_resolution = window->GetResolution();
	gameSettings->m_frameTimerEnabled = true;
}

void MainMenu::FrameStart()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainMenu::Update(float deltaTime)
{
}

void MainMenu::Render(float deltaTime)
{
	m_textEngine->Render("RaceCar Multiplayer", 330, 100, 3.f);

	switch (m_currentMenuState)
	{
	case MenuStates::INITIAL_MENU:
		m_goToChoseMenuButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_textEngine->Render(
			"CHOSE GAME TYPE",
			window->GetResolution().x / 2.f - 83.f,
			window->GetResolution().y / 2.f);

		break;
	case MenuStates::CHOSE_GAME_TYPE_MENU:
		m_goBackButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_textEngine->Render("GO BACK", 110, 615);

		m_singlePlayerChoseButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_textEngine->Render("SINGLEPLAYER", 360, 355);

		m_multiplayerChoseButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_textEngine->Render("MULTIPLAYER", 790, 355);

		break;
	case MenuStates::SINGLE_OR_MULTI_PLAYER_SPECIFIC_MENU:
		m_goBackButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_textEngine->Render("GO BACK", 110, 615);

		break;
	}
}

void MainMenu::FrameEnd()
{
}

void MainMenu::OnKeyPress(int key, int mods)
{
	teststring.push_back(static_cast<char>(key));
}

void MainMenu::OnKeyRelease(int key, int mods)
{
}

void MainMenu::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	switch (m_currentMenuState)
	{
	case MenuStates::INITIAL_MENU:
		if (m_goToChoseMenuButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
			IncrementMenuState();

		break;
	case MenuStates::CHOSE_GAME_TYPE_MENU:
		if (m_goBackButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
		{
			DecrementMenuState();
			m_gameMode = GameMode::UNSELECTED;
		}

		if (m_singlePlayerChoseButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
		{
			IncrementMenuState();
			m_gameMode = GameMode::SINGLEPLAYER;
			Scene::CloseScene();
		}

		if (m_multiplayerChoseButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
		{
			IncrementMenuState();
			m_gameMode = GameMode::MULTIPLAYER;
		}

		break;
	case MenuStates::SINGLE_OR_MULTI_PLAYER_SPECIFIC_MENU:
		if (m_goBackButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
			DecrementMenuState();

		break;
	}
}

void MainMenu::IncrementMenuState()
{
	m_currentMenuState = static_cast<MenuStates>(static_cast<int>(m_currentMenuState) + 1);
}

void MainMenu::DecrementMenuState()
{
	m_currentMenuState = static_cast<MenuStates>(static_cast<int>(m_currentMenuState) - 1);
}
