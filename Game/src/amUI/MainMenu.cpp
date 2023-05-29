#include "MainMenu.hpp"
#include "../../Server/src/DataArray.hpp"

#include <iostream>

MainMenu::MainMenu()
	: m_NDCSpace(-1.f, -1.f, 2.f, 2.f),
	  m_logicSpace(
		  0,
		  0,
		  static_cast<float>(window->GetResolution().x),
		  static_cast<float>(window->GetResolution().y))
{
	m_textEngine = std::make_unique<TextEngine>();
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
		auto defaultRectangle =
			std::make_shared<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red, true);
		auto textEngine = std::make_shared<TextEngine>();

		m_goBackButton = std::make_unique<RectangularButton>(
			defaultRectangle, 150.f, 100.f, 50.f, 25.f, textEngine, "Go back");
		m_goToChoseMenuButton = std::make_unique<RectangularButton>(
			defaultRectangle,
			window->GetResolution().x / 2.f,
			window->GetResolution().y / 2.f,
			100.f,
			50.f,
			textEngine,
			"Chose game mode");
		m_singlePlayerChoseButton = std::make_unique<RectangularButton>(
			defaultRectangle,
			window->GetResolution().x / 15.f * 5.f,
			window->GetResolution().y / 2.f,
			100.f,
			50.f,
			textEngine,
			"Singleplayer");
		m_multiplayerChoseButton = std::make_unique<RectangularButton>(
			defaultRectangle,
			window->GetResolution().x / 15.f * 10.f,
			window->GetResolution().y / 2.f,
			100.f,
			50.f,
			textEngine,
			"Multiplayer");
	}

	// Creare state menu si mod initial
	{
		m_currentMenuState = MenuStates::INITIAL_MENU;
		m_gameMode = GameMode::UNSELECTED;
	}

	// test = std::make_unique<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red);
}

void MainMenu::UpdateGameSettings()
{
	GameSettings* gameSettings = Engine::GetGameSettings();

	// TODO add UI entries for each settings
	gameSettings->InitParameters();

	gameSettings->m_gameMode = m_gameMode;

	gameSettings->m_resolution = window->GetResolution();
	
	gameSettings->m_frameTimerEnabled = true;
	gameSettings->m_forceFixedFramerate = true;
	gameSettings->m_nrOfFramesPerSecondForFixedFramerate = 60.f;

	if (m_gameMode == GameMode::MULTIPLAYER)
	{
		gameSettings->m_isMultiplayer = true;
		gameSettings->m_nrOfPlayers = maxNumberOfClients;
	}
	else
	{
		gameSettings->m_isMultiplayer = false;
		gameSettings->m_nrOfPlayers = 1;
	}
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
	m_textEngine->Render("RaceCar Multiplayer", 330, 540, 3.f);

	switch (m_currentMenuState)
	{
	case MenuStates::INITIAL_MENU:
		m_goToChoseMenuButton->Render(m_shader, m_logicToNDCSpaceMatrix);

		break;
	case MenuStates::CHOSE_GAME_TYPE_MENU:
		m_goBackButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_singlePlayerChoseButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_multiplayerChoseButton->Render(m_shader, m_logicToNDCSpaceMatrix);

		break;
	case MenuStates::SINGLE_OR_MULTI_PLAYER_SPECIFIC_MENU:
		m_goBackButton->Render(m_shader, m_logicToNDCSpaceMatrix);

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
		if (m_goToChoseMenuButton->HasBeenClicked(
				static_cast<float>(mouseX), static_cast<float>(window->GetResolution().y - mouseY)))
			IncrementMenuState();

		break;
	case MenuStates::CHOSE_GAME_TYPE_MENU:
		if (m_goBackButton->HasBeenClicked(
				static_cast<float>(mouseX), static_cast<float>(window->GetResolution().y - mouseY)))
		{
			DecrementMenuState();
			m_gameMode = GameMode::UNSELECTED;
		}

		if (m_singlePlayerChoseButton->HasBeenClicked(
				static_cast<float>(mouseX), static_cast<float>(window->GetResolution().y - mouseY)))
		{
			IncrementMenuState();
			m_gameMode = GameMode::SINGLEPLAYER;
			Scene::CloseScene();
		}

		if (m_multiplayerChoseButton->HasBeenClicked(
				static_cast<float>(mouseX), static_cast<float>(window->GetResolution().y - mouseY)))
		{
			IncrementMenuState();
			m_gameMode = GameMode::MULTIPLAYER;
			Scene::CloseScene();
		}

		break;
	case MenuStates::SINGLE_OR_MULTI_PLAYER_SPECIFIC_MENU:
		if (m_goBackButton->HasBeenClicked(
				static_cast<float>(mouseX), static_cast<float>(window->GetResolution().y - mouseY)))
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
