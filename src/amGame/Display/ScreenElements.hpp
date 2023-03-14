#pragma once

#include "Minimap.hpp"
#include "ProgressHandler.hpp"
#include "Speedometer.hpp"

class ScreenElements
{
public:
	ScreenElements() = delete;
	ScreenElements(
		const WindowObject* window,
		const std::shared_ptr<MiniMap>& miniMap,
		const std::shared_ptr<Speedometer>& speedometer,
		const std::shared_ptr<Car::PositionObeserver>& observer,
		const std::shared_ptr<Car::PlaceTracker>& placeTracker)
		: m_NDCSpace(-1.f, -1.f, 2.f, 2.f),
		  m_logicSpace(
			  0,
			  0,
			  static_cast<float>(window->GetResolution().x),
			  static_cast<float>(window->GetResolution().y)),
		  m_currentLap(1),
		  m_resetLap(false)
	{
		// Creare matrice de transformare
		m_transformMatrix = utils::VisualizationTransf2d(m_logicSpace, m_NDCSpace);

		// Copiere shared mem variables
		m_speedometer = speedometer;
		m_miniMap = miniMap;
		m_observer = observer;
		m_placeTracker = placeTracker;

		// Creare progress handler si text engine
		m_progressHandler = std::make_unique<ProgressHandler>(glm::vec3(1150, 350, 0), 300.f, 50.f);
		m_textEngine = std::make_unique<TextEngine>(window);

		// Cereare shader si frameminimap
		this->CreateShader(window);
		this->CreateMinimapFrame();
	}

	void Update()
	{
		float percent = m_observer->GetPercent();
		m_progressHandler->SetProgress(percent);
		if (!m_resetLap && percent > 99.25f)
		{
			m_currentLap++;
			m_resetLap = true;
		}
		else if (percent < 1.f)
		{
			m_resetLap = false;
		}
	}

	void Render() const
	{
		m_minimapFrame->Render(m_shader.get(), m_transformMatrix, glm::mat3(1));
		m_speedometer->Render(m_shader.get(), m_transformMatrix, glm::mat3(1));
		m_progressHandler->Render(m_shader.get(), m_transformMatrix, glm::mat3(1));

		m_textEngine->Render(
			"Current lap: " + std::to_string(m_currentLap) + " / 5", 20, 20, 1.0f, Colors::LightGray);

		std::pair<int, size_t> placeAndNr = m_placeTracker->GetPlaceAndNrOfNPCs();
		m_textEngine->Render(
			"Place: " + std::to_string(placeAndNr.first) + " / " + std::to_string(placeAndNr.second),
			20,
			50,
			1.0f,
			Colors::LightGray);
		m_textEngine->Render(
			"Start", m_progressHandler->GetPosition().x - 2.5f,
			m_progressHandler->GetPosition().y + 35,
			1.0f,
			Colors::LightGray);
		m_textEngine->Render(
			"Finish",
			m_progressHandler->GetPosition().x - 10.f,
			m_progressHandler->GetPosition().y - m_progressHandler->GetHeight() - 5.f,
			1.0f,
			Colors::LightGray);
	}

	void RenderCarRepresentation() const
	{
		m_miniMap->RenderCarRepresentation(m_shader.get(), m_transformMatrix, glm::mat3(1));
	}

private:
	void CreateShader(const WindowObject* window)
	{
		// Creare shader 2d
		m_shader = std::make_unique<Shader>("TestShader");
		m_shader->AddShader(
			PATH_JOIN(window->props.selfDir, SOURCE_PATH::MainMenu, "Shaders", "2dVertexShader.glsl"),
			GL_VERTEX_SHADER);
		m_shader->AddShader(
			PATH_JOIN(
				window->props.selfDir, SOURCE_PATH::MainMenu, "Shaders", "2dFragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		m_shader->CreateAndLink();
	}

	void CreateMinimapFrame()
	{
		// Setare propietati frame minimap
		m_minmapFrameColor = Colors::White;
		m_minimapFrameOutlineSize = 5.f;

		// Crearea propriu-zisa
		m_minimapFrame = std::make_unique<Polygon2d>(
			glm::vec2{
				m_miniMap->GetX() - m_minimapFrameOutlineSize,
				m_miniMap->GetY() - m_minimapFrameOutlineSize},
			m_miniMap->GetWidth() + m_minimapFrameOutlineSize * 2.f,
			m_miniMap->GetHeight() + m_minimapFrameOutlineSize * 2.f,
			m_minmapFrameColor);
	}

private:
	utils::LogicSpace m_NDCSpace;
	utils::LogicSpace m_logicSpace;

	glm::mat3 m_transformMatrix;

	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<TextEngine> m_textEngine;

	std::shared_ptr<Speedometer> m_speedometer;
	std::shared_ptr<MiniMap> m_miniMap;
	std::unique_ptr<ProgressHandler> m_progressHandler;
	std::shared_ptr<Car::PositionObeserver> m_observer;
	std::shared_ptr<Car::PlaceTracker> m_placeTracker;

	int m_currentLap;
	bool m_resetLap;

	std::unique_ptr<GeometryObject2d> m_minimapFrame;
	Color m_minmapFrameColor;
	float m_minimapFrameOutlineSize;
};