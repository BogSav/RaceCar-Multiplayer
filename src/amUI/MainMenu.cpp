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

	m_logicToNDCSpaceMatrix = utils::VisualizationTransf2d(m_logicSpace, m_NDCSpace);
}

void MainMenu::Init()
{
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

	m_optionsElement = std::make_unique<PolygonUIElement>(
		std::make_unique<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red, true), 10, 10, 200, 200);

	//test = std::make_unique<Polygon2d>(glm::vec2{-1.f, -1.f}, 2.f, 2.f, Colors::Red);
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
	m_optionsElement->Render(m_shader, m_logicToNDCSpaceMatrix);
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
	if (m_optionsElement->HasBeenClicked(mouseX, window->GetResolution().y - mouseY))
		std::cout << teststring << std::endl;
}
