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

	std::shared_ptr<Polygon2d> defaultRectangle =
		std::make_shared<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red, true);
	m_goBackButton = std::make_unique<PolygonUIElement>(defaultRectangle, 150, 100, 50, 25);
	m_goToChoseMenuButton = std::make_unique<PolygonUIElement>(
		defaultRectangle, window->GetResolution().x / 2, window->GetResolution().y / 2, 100, 50);

	m_currentMenuState = MenuStates::INITIAL_MENU;

	// test = std::make_unique<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red);
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
	switch (m_currentMenuState)
	{
	case MenuStates::INITIAL_MENU:
		m_goToChoseMenuButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_textEngine->Render(
			"CHOSE GAME TYPE",
			window->GetResolution().x / 2.f - 80,
			window->GetResolution().y / 2.f);
		break;
	case MenuStates::CHOSE_GAME_TYPE_MENU:
		m_goBackButton->Render(m_shader, m_logicToNDCSpaceMatrix);
		m_textEngine->Render("GO BACK", 150, 1000);
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
		if (m_goToChoseMenuButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
			m_currentMenuState = static_cast<MenuStates>(static_cast<int>(m_currentMenuState) + 1);
		break;
	case MenuStates::CHOSE_GAME_TYPE_MENU:
		if (m_goBackButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
			m_currentMenuState = static_cast<MenuStates>(static_cast<int>(m_currentMenuState) - 1);
		break;
	case MenuStates::SINGLE_OR_MULTI_PLAYER_SPECIFIC_MENU:
		if (m_goBackButton->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
			m_currentMenuState = static_cast<MenuStates>(static_cast<int>(m_currentMenuState) - 1);
		break;
	}
}
