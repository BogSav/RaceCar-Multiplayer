#pragma once

#include "ProgressHandler.hpp"
#include "Speedometer.hpp"
#include "amGame/GameComponents/Car.hpp"

class ScreenElements
{
public:
	ScreenElements() = delete;
	ScreenElements(const GameSettings* gameSettings, const Car* car)
		: m_NDCSpace(-1.f, -1.f, 2.f, 2.f),
		  m_logicSpace(
			  0.f,
			  0.f,
			  static_cast<float>(Engine::GetWindow()->GetResolution().x),
			  static_cast<float>(Engine::GetWindow()->GetResolution().y)),
		  m_car(car),
		  m_currentLap(1),
		  m_resetLap(false)
	{
		// Creare matrice de transformare
		m_transformMatrix = utils::VisualizationTransf2d(m_logicSpace, m_NDCSpace);

		m_textEngine = std::make_shared<TextEngine>();
		m_progressHandler = std::make_unique<ProgressHandler>(gameSettings, m_textEngine);
		m_speedometer = std::make_unique<Speedometer>(gameSettings, m_textEngine);

		// Cereare shader si frameminimap
		CreateShader();
	}

	void Update()
	{
		m_speedometer->SetGear(m_car->GetGear());
		m_speedometer->SetSpeed(m_car->GetSpeed());
		m_progressHandler->SetProgress(m_car->GetProgress());

		//float percent = m_observer->GetPercent();
		//m_progressHandler->SetProgress(percent);
		//if (!m_resetLap && percent > 99.25f)
		//{
		//	m_currentLap++;
		//	m_resetLap = true;
		//}
		//else if (percent < 1.f)
		//{
		//	m_resetLap = false;
		//}
	}

	void Render() const
	{
		m_speedometer->Render(m_shader.get(), m_transformMatrix, glm::mat3(1));
		m_progressHandler->Render(m_shader.get(), m_transformMatrix, glm::mat3(1));

		//m_textEngine->Render(
		//	"Current lap: " + std::to_string(m_currentLap) + " / 5",
		//	20,
		//	20,
		//	1.0f,
		//	Colors::LightGray);

		//m_textEngine->Render(
		//	"Place: " + std::to_string(placeAndNr.first) + " / "
		//		+ std::to_string(placeAndNr.second),
		//	20,
		//	50,
		//	1.0f,
		//	Colors::LightGray);
	}

private:
	void CreateShader()
	{
		// Creare shader 2d
		m_shader = std::make_unique<Shader>("TestShader");
		m_shader->AddShader(
			PATH_JOIN(
				Engine::GetWindow()->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"2dShaders",
				"2dVertexShader.glsl"),
			GL_VERTEX_SHADER);
		m_shader->AddShader(
			PATH_JOIN(
				Engine::GetWindow()->props.selfDir,
				SOURCE_PATH::CustomShaders,
				"2dShaders",
				"2dFragmentShader.glsl"),
			GL_FRAGMENT_SHADER);
		m_shader->CreateAndLink();
	}

private:
	utils::LogicSpace m_NDCSpace;
	utils::LogicSpace m_logicSpace;
	glm::mat3 m_transformMatrix;

	std::unique_ptr<Shader> m_shader;
	std::shared_ptr<TextEngine> m_textEngine;

	std::unique_ptr<Speedometer> m_speedometer;
	std::unique_ptr<ProgressHandler> m_progressHandler;

	const Car* m_car;

	int m_currentLap;
	bool m_resetLap;
};